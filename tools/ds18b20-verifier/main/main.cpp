/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"

#include "ocs_core/log.h"
#include "ocs_fmt/json/cjson_array_formatter.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_io/default_gpio.h"
#include "ocs_onewire/bus.h"
#include "ocs_onewire/rom_code.h"
#include "ocs_onewire/rom_code_scanner.h"
#include "ocs_onewire/serial_number_to_str.h"
#include "ocs_sensor/ds18b20/scratchpad.h"
#include "ocs_status/code_to_str.h"
#include "ocs_system/default_delayer.h"

using namespace ocs;

namespace {

const char* log_tag = "ds18b20_verifier";

struct VerifyParams {
    gpio_num_t gpio { GPIO_NUM_NC };
    unsigned total_attempts { 0 };
    TickType_t delay { pdMS_TO_TICKS(0) };
};

status::StatusCode read_temperature(onewire::Bus& bus, const onewire::RomCode& rom_code) {
    auto code = rom_code.match(bus);
    if (code != status::StatusCode::OK) {
        ocs_logi(log_tag, "failed to match ROM code: %s", status::code_to_str(code));
        return code;
    }

    code = rom_code.match(bus);
    if (code != status::StatusCode::OK) {
        ocs_logi(log_tag, "failed to match ROM code: %s", status::code_to_str(code));
        return code;
    }

    code = bus.write_byte(
        static_cast<uint8_t>(sensor::ds18b20::Scratchpad::Command::ConvertT));
    if (code != status::StatusCode::OK) {
        ocs_logi(log_tag, "failed to start temperature conversation: %s",
                 status::code_to_str(code));
        return code;
    }

    sensor::ds18b20::Scratchpad scratchpad;
    code = scratchpad.read(bus, rom_code);
    if (code != status::StatusCode::OK) {
        ocs_logi(log_tag, "failed to read scratchpad: %s", status::code_to_str(code));
        return code;
    }

    ocs_logi(log_tag, "serial_number=%s temperature=%.2f r0=%u r1=%u",
             onewire::serial_number_to_str(rom_code.serial_number).c_str(),
             scratchpad.get_temperature(), scratchpad.configuration.r0,
             scratchpad.configuration.r1);

    if (!scratchpad.valid()) {
        ocs_logi(log_tag, "failed to validate scratchpad");
        return status::StatusCode::InvalidState;
    }

    return status::StatusCode::OK;
}

void format_bus_params(fmt::json::CjsonObjectFormatter& formatter,
                       onewire::Bus::Params params) {
    configASSERT(
        formatter.add_number_cs("reset_pulse_interval", params.reset_pulse_interval));

    configASSERT(formatter.add_number_cs("presence_pulse_interval",
                                         params.presence_pulse_interval));

    configASSERT(
        formatter.add_number_cs("write_slot_interval", params.write_slot_interval));
    configASSERT(
        formatter.add_number_cs("write_bit_interval", params.write_bit_interval));

    configASSERT(formatter.add_number_cs("write_recovery_interval",
                                         params.write_recovery_interval));

    configASSERT(
        formatter.add_number_cs("read_slot_interval", params.read_slot_interval));
    configASSERT(
        formatter.add_number_cs("read_bit_init_interval", params.read_bit_init_interval));
    configASSERT(
        formatter.add_number_cs("read_bit_rc_interval", params.read_bit_rc_interval));

    configASSERT(
        formatter.add_number_cs("read_recovery_interval", params.read_recovery_interval));
}

void read_device(onewire::Bus& bus,
                 const onewire::RomCode& rom_code,
                 cJSON* json,
                 VerifyParams verify_params) {
    unsigned failed_attempts = 0;

    for (unsigned n = 0; n < verify_params.total_attempts; ++n) {
        const auto code = read_temperature(bus, rom_code);
        if (code != status::StatusCode::OK) {
            ++failed_attempts;
        }

        vTaskDelay(verify_params.delay);
    }

    fmt::json::CjsonObjectFormatter formatter(json);

    configASSERT(formatter.add_string_cs(
        "serial_number", onewire::serial_number_to_str(rom_code.serial_number).c_str()));

    configASSERT(formatter.add_number_cs("total_attempts", verify_params.total_attempts));
    configASSERT(formatter.add_number_cs("failed_attempts", failed_attempts));
}

void verify_bus_operations(VerifyParams verify_params, onewire::Bus::Params bus_params) {
    fmt::json::CjsonUniqueBuilder builder;

    auto json = builder.make_object();
    configASSERT(json);

    auto devices = cJSON_AddArrayToObject(json.get(), "devices");
    configASSERT(devices);

    fmt::json::CjsonObjectFormatter formatter(json.get());

    format_bus_params(formatter, bus_params);

    io::DefaultGpio gpio("test_GPIO_onewire_bus", verify_params.gpio);
    system::DefaultDelayer delayer;

    onewire::Bus bus(delayer, gpio, bus_params);

    onewire::RomCodeScanner scanner(bus);

    while (true) {
        onewire::RomCode rom_code;
        const auto code = scanner.scan(rom_code);
        if (code == status::StatusCode::NoData) {
            break;
        }

        if (code != status::StatusCode::OK) {
            ocs_loge(log_tag, "failed to scan ROM code: %s", status::code_to_str(code));
            return;
        }

        auto device_json = builder.make_object();
        configASSERT(device_json);

        read_device(bus, rom_code, device_json.get(), verify_params);

        configASSERT(cJSON_AddItemToArray(devices, device_json.get()));
        device_json.release();
    }

    fmt::json::DynamicFormatter json_formatter(
        CONFIG_OCS_TOOLS_DS18B20_VERIFIER_RESULT_BUFFER_SIZE);

    configASSERT(json_formatter.format(json.get()) == status::StatusCode::OK);

    ocs_logi(log_tag, "%s", json_formatter.c_str());
}

} // namespace

extern "C" void app_main(void) {
    verify_bus_operations(
        VerifyParams {
            .gpio = static_cast<gpio_num_t>(CONFIG_OCS_TOOLS_DS18B20_VERIFIER_GPIO),
            .total_attempts = CONFIG_OCS_TOOLS_DS18B20_VERIFIER_TOTAL_ATTEMPTS,
            .delay = pdMS_TO_TICKS(1000 * CONFIG_OCS_TOOLS_DS18B20_VERIFIER_DELAY),
        },
        onewire::Bus::Params {
            .reset_pulse_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_DS18B20_VERIFIER_RESET_PULSE_INTERVAL,
            .presence_pulse_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_DS18B20_VERIFIER_PRESENCE_PULSE_INTERVAL,
            .write_slot_interval =
                core::Microsecond * CONFIG_OCS_TOOLS_DS18B20_VERIFIER_WRITE_SLOT_INTERVAL,
            .write_bit_interval =
                core::Microsecond * CONFIG_OCS_TOOLS_DS18B20_VERIFIER_WRITE_BIT_INTERVAL,
            .write_recovery_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_DS18B20_VERIFIER_WRITE_RECOVERY_INTERVAL,
            .read_slot_interval =
                core::Microsecond * CONFIG_OCS_TOOLS_DS18B20_VERIFIER_READ_SLOT_INTERVAL,
            .read_bit_init_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_DS18B20_VERIFIER_READ_BIT_INIT_INTERVAL,
            .read_bit_rc_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_DS18B20_VERIFIER_READ_BIT_RC_INTERVAL,
            .read_recovery_interval = core::Microsecond
                * CONFIG_OCS_TOOLS_DS18B20_VERIFIER_READ_RECOVERY_INTERVAL,
        });

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
