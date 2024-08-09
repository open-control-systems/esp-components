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

LdrSensorJsonFormatter::LdrSensorJsonFormatter(sensor::LdrSensor& sensor)
    : sensor_(sensor) {
}

void LdrSensorJsonFormatter::format(cJSON* json) {
    CjsonObjectFormatter formatter(json);

    const auto data = sensor_.get_data();

    formatter.add_number_cs("sensor_ldr_raw", data.raw);
    formatter.add_number_cs("sensor_ldr_voltage", data.voltage);
    formatter.add_number_cs("sensor_ldr_lightness", data.lightness);
}

} // namespace iot
} // namespace ocs
