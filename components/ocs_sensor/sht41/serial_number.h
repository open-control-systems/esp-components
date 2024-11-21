/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>

#define SHT_SENSOR_SERIAL_NUMBER_LEN 4

#define SHT_SENSOR_SERIAL_NUMBER_STR "%02X:%02X:%02X:%02X"

#define SHT_SENSOR_SERIAL_NUMBER_TO_STR(serial_number)                                   \
    serial_number[0], serial_number[1], serial_number[2], serial_number[3]

namespace ocs {
namespace sensor {
namespace sht41 {

//! SHT sensor serial number, MSB byte order.
using SerialNumber = uint8_t[SHT_SENSOR_SERIAL_NUMBER_LEN];

} // namespace sht41
} // namespace sensor
} // namespace ocs
