/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_iot/ldr_sensor_json_formatter.h"
#include "ocs_iot/cjson_object_formatter.h"

namespace ocs {
namespace iot {

LdrSensorJsonFormatter::LdrSensorJsonFormatter(sensor::LdrSensor& sensor,
                                               bool flat_formatting)
    : BasicJsonFormatter(flat_formatting)
    , sensor_(sensor) {
}

status::StatusCode LdrSensorJsonFormatter::format(cJSON* json) {
    CjsonObjectFormatter formatter(json);

    const auto data = sensor_.get_data();

    if (flat_formatting_) {
        if (!formatter.add_number_cs("sensor_ldr_raw", data.raw)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_ldr_voltage", data.voltage)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_ldr_lightness", data.lightness)) {
            return status::StatusCode::NoMem;
        }
    } else {
        if (!formatter.add_number_cs("raw", data.raw)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("voltage", data.voltage)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("lightness", data.lightness)) {
            return status::StatusCode::NoMem;
        }
    }

    return status::StatusCode::OK;
}

} // namespace iot
} // namespace ocs
