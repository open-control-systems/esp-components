/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/ds18b20/json_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

JsonFormatter::JsonFormatter(Sensor& sensor)
    : sensor_(sensor) {
}

status::StatusCode JsonFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    if (!formatter.add_number_cs(sensor_.id(), sensor_.get_data())) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
