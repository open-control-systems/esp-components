/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_iot/ijson_formatter.h"
#include "ocs_sensor/yl69_sensor.h"

namespace ocs {
namespace iot {

class YL69SensorJsonFormatter : public IJsonFormatter, public core::NonCopyable<> {
public:
    //! Initialize.
    explicit YL69SensorJsonFormatter(sensor::YL69Sensor& sensor);

    //! Format YL69 sensor data into @p json.
    void format(cJSON* json) override;

private:
    sensor::YL69Sensor& sensor_;
};

} // namespace iot
} // namespace ocs
