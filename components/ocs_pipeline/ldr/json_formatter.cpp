/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/ldr/json_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace pipeline {
namespace ldr {

JsonFormatter::JsonFormatter(sensor::LdrSensor& sensor, bool flat_formatting)
    : BasicFormatter(flat_formatting)
    , sensor_(sensor) {
}

status::StatusCode JsonFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

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

} // namespace ldr
} // namespace pipeline
} // namespace ocs
