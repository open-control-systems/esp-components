/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <charconv>
#include <cstring>

#include "ocs_algo/uri_ops.h"
#include "ocs_core/operation_guard.h"
#include "ocs_fmt/json/cjson_array_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_onewire/rom_code.h"
#include "ocs_onewire/rom_code_scanner.h"
#include "ocs_onewire/serial_number_to_str.h"
#include "ocs_pipeline/httpserver/ds18b20_handler.h"
#include "ocs_sensor/ds18b20/parse_configuration.h"
#include "ocs_sensor/ds18b20/resolution_to_str.h"
#include "ocs_system/suspender_guard.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

namespace {

sensor::ds18b20::Sensor* get_sensor(const std::string_view& sensor_id,
                                    const sensor::ds18b20::Store::SensorList& sensors) {
    for (const auto& sensor : sensors) {
        if (sensor_id == sensor->id()) {
            return sensor;
        }
    }

    return nullptr;
}

status::StatusCode
format_configuration(cJSON* json,
                     const sensor::ds18b20::Sensor::Configuration& configuration) {
    fmt::json::CjsonObjectFormatter formatter(json);

    if (!formatter.add_string_cs(
            "resolution", sensor::ds18b20::resolution_to_str(configuration.resolution))) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_string_cs(
            "serial_number",
            onewire::serial_number_to_str(configuration.rom_code.serial_number)
                .c_str())) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

status::StatusCode scan_rom_codes(fmt::json::CjsonArrayFormatter& formatter,
                                  onewire::Bus& bus) {
    onewire::RomCodeScanner scanner(bus);

    while (true) {
        onewire::RomCode rom_code;

        const auto code = scanner.scan(rom_code);
        if (code != status::StatusCode::OK) {
            if (code != status::StatusCode::NoData) {
                return code;
            }

            break;
        }

        onewire::serial_number_to_str str(rom_code.serial_number);
        if (!formatter.append_string(str.c_str())) {
            return status::StatusCode::NoMem;
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode find_rom_code(onewire::Bus& bus,
                                 sensor::ds18b20::Sensor::Configuration& configuration) {
    onewire::RomCodeScanner scanner(bus);

    while (true) {
        onewire::RomCode rom_code;

        const auto code = scanner.scan(rom_code);
        if (code != status::StatusCode::OK) {
            return code;
        }

        if (memcmp(rom_code.serial_number, configuration.rom_code.serial_number,
                   sizeof(rom_code.serial_number))
            == 0) {
            configuration.rom_code = rom_code;
            break;
        }
    }

    return status::StatusCode::OK;
}

} // namespace

DS18B20Handler::DS18B20Handler(http::Server& server,
                               net::IMdnsDriver& mdns_driver,
                               system::ISuspender& suspender,
                               sensor::ds18b20::Store& store)
    : suspender_(suspender)
    , store_(store) {
    server.add_GET("/sensor/ds18b20/scan", [this](httpd_req_t* req) {
        return handle_scan_(req);
    });
    server.add_GET("/sensor/ds18b20/read_configuration", [this](httpd_req_t* req) {
        return handle_configuration_(
            req, read_wait_interval_, read_response_buffer_size_,
            [this](cJSON* json, sensor::ds18b20::Sensor& sensor) {
                return read_configuration_(json, sensor);
            });
    });
    server.add_GET("/sensor/ds18b20/write_configuration", [this](httpd_req_t* req) {
        return handle_write_configuration_(req);
    });
    server.add_GET("/sensor/ds18b20/erase_configuration", [this](httpd_req_t* req) {
        return handle_configuration_(
            req, erase_wait_interval_, erase_response_buffer_size_,
            [this](cJSON* json, sensor::ds18b20::Sensor& sensor) {
                return erase_configuration_(json, sensor);
            });
    });

    configASSERT(mdns_driver.add_txt_record(net::IMdnsDriver::Service::Http,
                                            net::IMdnsDriver::Proto::Tcp,
                                            "sensor_ds18b20_scan", "/sensor/ds18b20/scan")
                 == status::StatusCode::OK);

    configASSERT(mdns_driver.add_txt_record(net::IMdnsDriver::Service::Http,
                                            net::IMdnsDriver::Proto::Tcp,
                                            "sensor_ds18b20_read_configuration",
                                            "/sensor/ds18b20/read_configuration")
                 == status::StatusCode::OK);

    configASSERT(mdns_driver.add_txt_record(net::IMdnsDriver::Service::Http,
                                            net::IMdnsDriver::Proto::Tcp,
                                            "sensor_ds18b20_write_configuration",
                                            "/sensor/ds18b20/write_configuration")
                 == status::StatusCode::OK);

    configASSERT(mdns_driver.add_txt_record(net::IMdnsDriver::Service::Http,
                                            net::IMdnsDriver::Proto::Tcp,
                                            "sensor_ds18b20_erase_configuration",
                                            "/sensor/ds18b20/erase_configuration")
                 == status::StatusCode::OK);
}

status::StatusCode DS18B20Handler::handle_scan_(httpd_req_t* req) {
    const auto values = algo::UriOps::parse_query(req->uri);
    const auto it = values.find("gpio");
    if (it == values.end()) {
        return status::StatusCode::InvalidArg;
    }

    int gpio = 0;

    const auto [_, ec] =
        std::from_chars(it->second.data(), it->second.data() + it->second.size(), gpio);
    if (ec != std::errc()) {
        return status::StatusCode::InvalidArg;
    }

    fmt::json::CjsonUniqueBuilder builder;

    auto json = builder.make_object();
    if (!json) {
        return status::StatusCode::NoMem;
    }

    auto future = store_.schedule(
        static_cast<io::gpio::Gpio>(gpio),
        [this, &json, &builder](onewire::Bus& bus,
                                sensor::ds18b20::Store::SensorList& sensors) {
            return scan_(json.get(), builder, bus, sensors);
        });
    if (!future) {
        return status::StatusCode::InvalidState;
    }
    if (future->wait(scan_wait_interval_) != status::StatusCode::OK) {
        return status::StatusCode::Timeout;
    }
    if (future->code() != status::StatusCode::OK) {
        return future->code();
    }

    return send_response_(scan_response_buffer_size_, json.get(), req);
}

status::StatusCode
DS18B20Handler::scan_(cJSON* json,
                      fmt::json::CjsonUniqueBuilder& builder,
                      onewire::Bus& bus,
                      const sensor::ds18b20::Store::SensorList& sensors) {
    auto code = scan_rom_code_(json, bus);
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = format_sensors_(json, builder, sensors);
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode DS18B20Handler::scan_rom_code_(cJSON* json, onewire::Bus& bus) {
    auto array = cJSON_AddArrayToObject(json, "rom_codes");
    if (!array) {
        return status::StatusCode::NoMem;
    }

    fmt::json::CjsonArrayFormatter formatter(array);

    system::SuspenderGuard suspender_guard(suspender_);
    core::OperationGuard operation_guard;

    return scan_rom_codes(formatter, bus);
}

status::StatusCode
DS18B20Handler::format_sensors_(cJSON* json,
                                fmt::json::CjsonUniqueBuilder& builder,
                                const sensor::ds18b20::Store::SensorList& sensors) {
    auto array = cJSON_AddArrayToObject(json, "sensors");
    if (!array) {
        return status::StatusCode::NoMem;
    }

    fmt::json::CjsonArrayFormatter formatter(array);

    for (const auto& sensor : sensors) {
        const auto code = format_sensor_(array, builder, *sensor);
        if (code != status::StatusCode::OK) {
            return code;
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode DS18B20Handler::format_sensor_(cJSON* array,
                                                  fmt::json::CjsonUniqueBuilder& builder,
                                                  const sensor::ds18b20::Sensor& sensor) {
    auto json = builder.make_object();
    if (!json) {
        return status::StatusCode::NoMem;
    }

    fmt::json::CjsonObjectFormatter formatter(json.get());

    if (!formatter.add_string_cs("id", sensor.id())) {
        return status::StatusCode::NoMem;
    }

    if (sensor.configured()) {
        if (!formatter.add_true_cs("configured")) {
            return status::StatusCode::NoMem;
        }
    } else {
        if (!formatter.add_false_cs("configured")) {
            return status::StatusCode::NoMem;
        }
    }

    if (!cJSON_AddItemToArray(array, json.get())) {
        return status::StatusCode::NoMem;
    }
    json.release();

    return status::StatusCode::OK;
}

status::StatusCode
DS18B20Handler::handle_configuration_(httpd_req_t* req,
                                      unsigned wait_interval,
                                      unsigned response_size,
                                      DS18B20Handler::HandleConfigurationFunc func) {
    const auto values = algo::UriOps::parse_query(req->uri);

    const auto gpio_it = values.find("gpio");
    if (gpio_it == values.end()) {
        return status::StatusCode::InvalidArg;
    }

    int gpio = 0;

    const auto [_, ec] = std::from_chars(
        gpio_it->second.data(), gpio_it->second.data() + gpio_it->second.size(), gpio);
    if (ec != std::errc()) {
        return status::StatusCode::InvalidArg;
    }

    const auto sensor_id = values.find("sensor_id");
    if (sensor_id == values.end()) {
        return status::StatusCode::InvalidArg;
    }

    fmt::json::CjsonUniqueBuilder builder;

    auto json = builder.make_object();
    if (!json) {
        return status::StatusCode::NoMem;
    }

    auto future = store_.schedule(
        static_cast<io::gpio::Gpio>(gpio),
        [this, &json, &sensor_id, func](onewire::Bus& bus,
                                        sensor::ds18b20::Store::SensorList& sensors) {
            auto sensor = get_sensor(sensor_id->second, sensors);
            if (!sensor) {
                return status::StatusCode::InvalidArg;
            }

            return func(json.get(), *sensor);
        });
    if (!future) {
        return status::StatusCode::InvalidState;
    }
    if (future->wait(wait_interval) != status::StatusCode::OK) {
        return status::StatusCode::Timeout;
    }
    if (future->code() != status::StatusCode::OK) {
        return future->code();
    }

    return send_response_(response_size, json.get(), req);
}

status::StatusCode DS18B20Handler::read_configuration_(cJSON* json,
                                                       sensor::ds18b20::Sensor& sensor) {
    sensor::ds18b20::Sensor::Configuration configuration;
    auto code = sensor.read_configuration(configuration);
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        return code;
    }

    code = format_configuration(json, configuration);
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode DS18B20Handler::handle_write_configuration_(httpd_req_t* req) {
    const auto values = algo::UriOps::parse_query(req->uri);

    const auto gpio_it = values.find("gpio");
    if (gpio_it == values.end()) {
        return status::StatusCode::InvalidArg;
    }

    int gpio = 0;

    const auto [_, ec] = std::from_chars(
        gpio_it->second.data(), gpio_it->second.data() + gpio_it->second.size(), gpio);
    if (ec != std::errc()) {
        return status::StatusCode::InvalidArg;
    }

    const auto sensor_id = values.find("sensor_id");
    if (sensor_id == values.end()) {
        return status::StatusCode::InvalidArg;
    }

    const auto serial_number = values.find("serial_number");
    if (serial_number == values.end()) {
        return status::StatusCode::InvalidArg;
    }

    const auto resolution = values.find("resolution");
    if (resolution == values.end()) {
        return status::StatusCode::InvalidArg;
    }

    fmt::json::CjsonUniqueBuilder builder;

    auto json = builder.make_object();
    if (!json) {
        return status::StatusCode::NoMem;
    }

    auto future = store_.schedule(
        static_cast<io::gpio::Gpio>(gpio),
        [this, &json, &sensor_id, &serial_number,
         &resolution](onewire::Bus& bus, sensor::ds18b20::Store::SensorList& sensors) {
            return write_configuration_(json.get(), bus,
                                        get_sensor(sensor_id->second, sensors),
                                        serial_number->second, resolution->second);
        });
    if (!future) {
        return status::StatusCode::InvalidState;
    }
    if (future->wait(write_wait_interval_) != status::StatusCode::OK) {
        return status::StatusCode::Timeout;
    }
    if (future->code() != status::StatusCode::OK) {
        return future->code();
    }

    return send_response_(write_response_buffer_size_, json.get(), req);
}

status::StatusCode
DS18B20Handler::write_configuration_(cJSON* json,
                                     onewire::Bus& bus,
                                     sensor::ds18b20::Sensor* sensor,
                                     const std::string_view& serial_number,
                                     const std::string_view& resolution) {
    if (!sensor) {
        return status::StatusCode::InvalidArg;
    }

    sensor::ds18b20::Sensor::Configuration configuration;

    auto code =
        sensor::ds18b20::parse_configuration(configuration, serial_number, resolution);
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = find_rom_code_(bus, configuration);
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = sensor->write_configuration(bus, configuration);
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = read_configuration_(json, *sensor);
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode
DS18B20Handler::find_rom_code_(onewire::Bus& bus,
                               sensor::ds18b20::Sensor::Configuration& configuration) {
    system::SuspenderGuard suspender_guard(suspender_);

    core::OperationGuard operation_guard;

    return find_rom_code(bus, configuration);
}

status::StatusCode DS18B20Handler::erase_configuration_(cJSON* json,
                                                        sensor::ds18b20::Sensor& sensor) {
    fmt::json::CjsonObjectFormatter formatter(json);

    sensor::ds18b20::Sensor::Configuration configuration;

    auto code = sensor.erase_configuration(configuration);
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        return code;
    }

    code = format_configuration(json, configuration);
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode
DS18B20Handler::send_response_(unsigned buffer_size, cJSON* json, httpd_req_t* req) {
    fmt::json::DynamicFormatter json_formatter(buffer_size);

    const auto code = json_formatter.format(json);
    if (code != status::StatusCode::OK) {
        return code;
    }

    const auto err = httpd_resp_send(req, json_formatter.c_str(), HTTPD_RESP_USE_STRLEN);
    if (err != ESP_OK) {
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
