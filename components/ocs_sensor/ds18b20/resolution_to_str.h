/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_sensor/ds18b20/sensor.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

//! Return human-readable resolution description.
const char* resolution_to_str(Sensor::Configuration::Resolution resolution);

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
