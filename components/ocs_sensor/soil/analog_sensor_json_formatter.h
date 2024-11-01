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
#include "ocs_sensor/soil/analog_sensor.h"

namespace ocs {
namespace sensor {
namespace soil {

class AnalogSensorJsonFormatter : public fmt::json::BasicFormatter,
                                  public core::NonCopyable<> {
public:
    //! Initialize.
    AnalogSensorJsonFormatter(AnalogSensor& sensor, bool flat_formatting = true);

    //! Format soil sensor data into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    AnalogSensor& sensor_;
};

} // namespace soil
} // namespace sensor
} // namespace ocs