/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/noncopyable.h"
#include "ocs_i2c/itransceiver.h"
#include "ocs_scheduler/itask.h"
#include "ocs_sensor/basic_sensor.h"

namespace ocs {
namespace sensor {
namespace sht41 {

struct SensorData {
    double humidity { 0.0 };
    double temperature { 0.0 };
};

//! Read data from SHT41 sensor.
//!
//! @reference
//!  - https://sensirion.com/products/catalog/SEK-SHT41
class Sensor : public BasicSensor<SensorData>,
               public scheduler::ITask,
               public core::NonCopyable<> {
public:
    enum class MeasureMode : uint8_t {
        HighPrecision = 0xFD,
        MediumPrecision = 0xF6,
        LowPrecision = 0xE0,
    };

    struct Params {
        //! How long to wait before receiving the operation result from the I2C device.
        TickType_t send_wait_interval { pdMS_TO_TICKS(10) };

        //! How long to wait for I2C operation to complete.
        core::Time bus_wait_interval { core::Second * 5 };

        //! How precise data should be measured.
        MeasureMode measure_mode { MeasureMode::HighPrecision };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p transceiver to communicate with the I2C device.
    //!  - @p id to distinguish one sensor from another.
    Sensor(i2c::ITransceiver& transceiver, const char* id, Params params);

    //! Read sensor data.
    status::StatusCode run() override;

private:
    const Params params_;

    i2c::ITransceiver& transceiver_;
};

} // namespace sht41
} // namespace sensor
} // namespace ocs
