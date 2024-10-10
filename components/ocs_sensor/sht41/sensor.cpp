/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "ocs_sensor/sht41/sensor.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace sensor {
namespace sht41 {

Sensor::Sensor(i2c::ITransmitter& transmitter)
    : transmitter_(transmitter) {
}

status::StatusCode Sensor::run() {
    Command command = Command::ReadSerialNumber;

    // Transmit command.
    OCS_STATUS_RETURN_ON_ERROR(
        transmitter_.write(reinterpret_cast<const uint8_t*>(&command), , -1));

    // from the transmitter.
    uint8_t serial_number[6];
    memset(serial_number, 0, sizeof(serial_number));

    return status::StatusCode::Error;
}

} // namespace sht41
} // namespace sensor
} // namespace ocs
