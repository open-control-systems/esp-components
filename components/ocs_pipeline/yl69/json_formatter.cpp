/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/yl69/json_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_sensor/yl69/soil_status_to_str.h"

namespace ocs {
namespace pipeline {
namespace yl69 {

JsonFormatter::JsonFormatter(sensor::yl69::Sensor& sensor, bool flat_formatting)
    : BasicFormatter(flat_formatting)
    , sensor_(sensor) {
}

status::StatusCode JsonFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    const auto data = sensor_.get_data();

    if (flat_formatting_) {
        if (!formatter.add_number_cs("sensor_yl69_raw", data.raw)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_yl69_voltage", data.voltage)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_yl69_moisture", data.moisture)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_string_ref_cs(
                "sensor_yl69_prev_status",
                sensor::yl69::soil_status_to_str(data.prev_status))) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_yl69_prev_status_dur",
                                     data.prev_status_duration)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_string_ref_cs(
                "sensor_yl69_curr_status",
                sensor::yl69::soil_status_to_str(data.curr_status))) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_yl69_curr_status_dur",
                                     data.curr_status_duration)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_yl69_write_count", data.write_count)) {
            return status::StatusCode::NoMem;
        }
    } else {
        if (!formatter.add_number_cs("raw", data.raw)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("voltage", data.voltage)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("moisture", data.moisture)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_string_ref_cs(
                "sensor_yl69_prev_status",
                sensor::yl69::soil_status_to_str(data.prev_status))) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_yl69_prev_status_dur",
                                     data.prev_status_duration)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_string_ref_cs(
                "sensor_yl69_curr_status",
                sensor::yl69::soil_status_to_str(data.curr_status))) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_yl69_curr_status_dur",
                                     data.curr_status_duration)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("write_count", data.write_count)) {
            return status::StatusCode::NoMem;
        }
    }

    return status::StatusCode::OK;
}

} // namespace yl69
} // namespace pipeline
} // namespace ocs
