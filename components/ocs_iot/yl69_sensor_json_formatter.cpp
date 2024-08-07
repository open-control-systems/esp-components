/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_iot/yl69_sensor_json_formatter.h"
#include "ocs_iot/cjson_object_formatter.h"

namespace ocs {
namespace iot {

YL69SensorJsonFormatter::YL69SensorJsonFormatter(sensor::YL69Sensor& sensor)
    : sensor_(sensor) {
}

void YL69SensorJsonFormatter::format(cJSON* json) {
    CjsonObjectFormatter formatter(json);

    const auto data = sensor_.get_data();

    formatter.add_number_cs("sensor_yl69_raw", data.raw);
    formatter.add_number_cs("sensor_yl69_voltage", data.voltage);

    formatter.add_string_ref_cs("sensor_yl69_status",
                                sensor::YL69Sensor::soil_status_to_str(data.status));
}

} // namespace iot
} // namespace ocs
