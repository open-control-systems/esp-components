/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string_view>

#include "ocs_sensor/ds18b20/sensor.h"
#include "ocs_status/code.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

//! Parse sensor configuration from @p serial_number and @p resolution.
status::StatusCode parse_configuration(Sensor::Configuration& configuration,
                                       const std::string_view& serial_number,
                                       const std::string_view& resolution);

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
