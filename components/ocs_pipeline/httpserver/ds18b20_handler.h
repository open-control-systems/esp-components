/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <functional>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_http/server.h"
#include "ocs_net/mdns_provider.h"
#include "ocs_sensor/ds18b20/store.h"
#include "ocs_system/isuspender.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class DS18B20Handler : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register endpoints.
    //!  - @p provider to register mDNS txt records.
    //!  - @p suspender to suspend the system during sensors operations.
    //!  - @p store to perform operations on sensors.
    DS18B20Handler(http::Server& server,
                   net::MdnsProvider& prover,
                   system::ISuspender& suspender,
                   sensor::ds18b20::Store& store);

private:
    using HandleConfigurationFunc =
        std::function<status::StatusCode(cJSON*, sensor::ds18b20::Sensor&)>;

    static const unsigned scan_response_buffer_size_ { 256 };
    static const unsigned read_response_buffer_size_ { 256 };
    static const unsigned write_response_buffer_size_ { 256 };
    static const unsigned erase_response_buffer_size_ { 256 };

    static const TickType_t scan_wait_interval_ { pdMS_TO_TICKS(10 * 1000) };
    static const TickType_t read_wait_interval_ { pdMS_TO_TICKS(5 * 1000) };
    static const TickType_t write_wait_interval_ { pdMS_TO_TICKS(5 * 1000) };
    static const TickType_t erase_wait_interval_ { pdMS_TO_TICKS(5 * 1000) };

    status::StatusCode handle_scan_(httpd_req_t* req);

    status::StatusCode scan_(cJSON* json,
                             fmt::json::CjsonUniqueBuilder& builder,
                             onewire::Bus& bus,
                             const sensor::ds18b20::Store::SensorList& sensors);

    status::StatusCode scan_rom_code_(cJSON* json, onewire::Bus& bus);

    status::StatusCode format_sensors_(cJSON* json,
                                       fmt::json::CjsonUniqueBuilder& builder,
                                       const sensor::ds18b20::Store::SensorList& sensors);

    status::StatusCode format_sensor_(cJSON* json,
                                      fmt::json::CjsonUniqueBuilder& builder,
                                      const sensor::ds18b20::Sensor& sensors);

    status::StatusCode handle_configuration_(httpd_req_t* req,
                                             unsigned wait_interval,
                                             unsigned response_size,
                                             HandleConfigurationFunc func);

    status::StatusCode read_configuration_(cJSON* json, sensor::ds18b20::Sensor&);

    status::StatusCode handle_write_configuration_(httpd_req_t* req);

    status::StatusCode write_configuration_(cJSON* json,
                                            onewire::Bus& bus,
                                            sensor::ds18b20::Sensor* sensor,
                                            const std::string_view& serial_number,
                                            const std::string_view& resolution);

    status::StatusCode
    find_rom_code_(onewire::Bus& bus,
                   sensor::ds18b20::Sensor::Configuration& configuration);

    status::StatusCode erase_configuration_(cJSON* json, sensor::ds18b20::Sensor& sensor);

    status::StatusCode
    send_response_(unsigned buffer_size, cJSON* json, httpd_req_t* req);

    system::ISuspender& suspender_;
    sensor::ds18b20::Store& store_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
