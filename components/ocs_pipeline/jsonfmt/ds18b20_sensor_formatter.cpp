/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/jsonfmt/ds18b20_sensor_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

DS18B20SensorFormatter::DS18B20SensorFormatter(sensor::ds18b20::Sensor& sensor)
    : sensor_(sensor) {
}

status::StatusCode DS18B20SensorFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    if (!formatter.add_number_cs(sensor_.id(), sensor_.get_data())) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
