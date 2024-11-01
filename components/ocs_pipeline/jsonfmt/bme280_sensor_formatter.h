/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/basic_formatter.h"
#include "ocs_sensor/bme280/sensor.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class BME280SensorFormatter : public fmt::json::BasicFormatter,
                              public core::NonCopyable<> {
public:
    //! Initialize.
    BME280SensorFormatter(sensor::bme280::Sensor& sensor, bool flat_formatting = true);

    //! Format bme280 sensor data into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    sensor::bme280::Sensor& sensor_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
