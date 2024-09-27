/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_sensor/yl69/soil_status.h"

namespace ocs {
namespace sensor {
namespace yl69 {

//! Convert soil moisture status to human-readable description.
const char* soil_status_to_str(SoilStatus);

} // namespace yl69
} // namespace sensor
} // namespace ocs
