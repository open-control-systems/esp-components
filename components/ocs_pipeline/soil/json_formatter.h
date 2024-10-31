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
#include "ocs_sensor/soil/sensor.h"

namespace ocs {
namespace pipeline {
namespace soil {

class JsonFormatter : public fmt::json::BasicFormatter, public core::NonCopyable<> {
public:
    //! Initialize.
    JsonFormatter(sensor::soil::Sensor& sensor, bool flat_formatting = true);

    //! Format soil sensor data into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    sensor::soil::Sensor& sensor_;
};

} // namespace soil
} // namespace pipeline
} // namespace ocs
