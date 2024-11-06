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
#include "ocs_core/spmc_node.h"
#include "ocs_io/i2c/itransceiver.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace sensor {
namespace sht41 {

//! Read data from SHT41 sensor.
//!
//! @reference
//!  - https://sensirion.com/products/catalog/SEK-SHT41
class Sensor : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Various sensor characteristics.
    struct Data {
        double humidity { 0.0 };
        double temperature { 0.0 };
    };

    enum class MeasureMode : uint8_t {
        HighPrecision = 0xFD,
        MediumPrecision = 0xF6,
        LowPrecision = 0xE0,
    };

    struct Params {
        //! How long to wait before receiving the operation result from the I2C device.
        TickType_t send_wait_interval { pdMS_TO_TICKS(10) };

        //! How long to wait for I2C operation to complete.
        core::Time bus_wait_interval { core::Duration::second * 5 };

        //! How precise data should be measured.
        MeasureMode measure_mode { MeasureMode::HighPrecision };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p transceiver to communicate with the I2C device.
    Sensor(io::i2c::ITransceiver& transceiver, Params params);

    //! Read sensor data.
    status::StatusCode run() override;

    //! Return the latest sensor data.
    Data get_data() const;

private:
    const Params params_;

    io::i2c::ITransceiver& transceiver_;

    core::SpmcNode<Data> data_;
};

} // namespace sht41
} // namespace sensor
} // namespace ocs
