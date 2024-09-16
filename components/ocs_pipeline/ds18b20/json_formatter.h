/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_iot/ijson_formatter.h"
#include "ocs_sensor/ds18b20/sensor.h"

namespace ocs {
namespace pipeline {
namespace ds18b20 {

class JsonFormatter : public iot::IJsonFormatter, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p sensor to read temperature.
    explicit JsonFormatter(sensor::ds18b20::Sensor& sensor);

    //! Format DS18B20 sensor data into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    sensor::ds18b20::Sensor& sensor_;
};

} // namespace ds18b20
} // namespace pipeline
} // namespace ocs
