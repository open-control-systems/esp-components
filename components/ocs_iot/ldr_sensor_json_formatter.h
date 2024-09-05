/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_iot/basic_json_formatter.h"
#include "ocs_sensor/ldr_sensor.h"

namespace ocs {
namespace iot {

class LdrSensorJsonFormatter : public BasicJsonFormatter, public core::NonCopyable<> {
public:
    //! Initialize.
    LdrSensorJsonFormatter(sensor::LdrSensor& sensor, bool flat_formatting);

    //! Format LDR sensor data into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    sensor::LdrSensor& sensor_;
};

} // namespace iot
} // namespace ocs
