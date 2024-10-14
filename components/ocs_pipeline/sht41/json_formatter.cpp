/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/sht41/json_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace pipeline {
namespace sht41 {

JsonFormatter::JsonFormatter(sensor::sht41::Sensor& sensor, bool flat_formatting)
    : BasicFormatter(flat_formatting)
    , sensor_(sensor) {
}

status::StatusCode JsonFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    const auto data = sensor_.get_data();

    if (flat_formatting_) {
        if (!formatter.add_number_cs("sensor_sht41_humidity", data.humidity)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("sensor_sht41_temperature", data.temperature)) {
            return status::StatusCode::NoMem;
        }
    } else {
        if (!formatter.add_number_cs("humidity", data.humidity)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("temperature", data.temperature)) {
            return status::StatusCode::NoMem;
        }
    }

    return status::StatusCode::OK;
}

} // namespace sht41
} // namespace pipeline
} // namespace ocs
