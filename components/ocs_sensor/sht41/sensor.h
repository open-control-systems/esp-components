/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_i2c/itransmitter.h"
#include "ocs_scheduler/itask.h"
#include "ocs_sensor/basic_sensor.h"

namespace ocs {
namespace sensor {
namespace sht41 {

struct SensorData {
    float relative_humidity { 0.0 };
    float temperature { 0.0 };
};

//! Read data from SHT41 sensor.
//!
//! @reference
//!  - https://sensirion.com/products/catalog/SEK-SHT41
class Sensor : public BasicSensor<SensorData>,
               public scheduler::ITask,
               public core::NonCopyable<> {
public:
    enum class Command : uint8_t {
        MeasureHighPrecision = 0xFD,
        MeasureMediumPrecision = 0xF6,
        MeasureLowPrecision = 0xE0,
        ReadSerialNumber = 0x89,
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p transmitter to communicate with the I2C device.
    explicit Sensor(i2c::ITransmitter& transmitter);

    //! Read sensor data.
    status::StatusCode run() override;

private:
    i2c::ITransmitter& transmitter_;
};

} // namespace sht41
} // namespace sensor
} // namespace ocs
