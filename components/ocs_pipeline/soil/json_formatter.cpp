/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/soil/json_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_sensor/soil/soil_status_to_str.h"

namespace ocs {
namespace pipeline {
namespace soil {

JsonFormatter::JsonFormatter(sensor::soil::AnalogSensor& sensor, bool flat_formatting)
    : BasicFormatter(flat_formatting)
    , sensor_(sensor) {
}

status::StatusCode JsonFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    const auto data = sensor_.get_data();

    if (flat_formatting_) {
        if (!formatter.add_number_cs("sensor_soil_raw", data.raw)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_soil_voltage", data.voltage)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_soil_moisture", data.moisture)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_string_ref_cs(
                "sensor_soil_prev_status",
                sensor::soil::soil_status_to_str(data.prev_status))) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_soil_prev_status_dur",
                                     data.prev_status_duration)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_string_ref_cs(
                "sensor_soil_curr_status",
                sensor::soil::soil_status_to_str(data.curr_status))) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_soil_curr_status_dur",
                                     data.curr_status_duration)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_soil_write_count", data.write_count)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_soil_status_len", data.status_len)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_soil_status_pos", data.status_pos)) {
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
                "prev_status", sensor::soil::soil_status_to_str(data.prev_status))) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("prev_status_dur", data.prev_status_duration)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_string_ref_cs(
                "curr_status", sensor::soil::soil_status_to_str(data.curr_status))) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("curr_status_dur", data.curr_status_duration)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("write_count", data.write_count)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("status_len", data.status_len)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("status_pos", data.status_pos)) {
            return status::StatusCode::NoMem;
        }
    }

    return status::StatusCode::OK;
}

} // namespace soil
} // namespace pipeline
} // namespace ocs
