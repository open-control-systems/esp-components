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
#include "ocs_storage/istorage.h"

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
        unsigned heating_count { 0 };
    };

    enum class Command {
        MeasureHighPrecision = 0xFD,
        MeasureMediumPrecision = 0xF6,
        MeasureLowPrecision = 0xE0,
        ReadSerialNumber = 0x89,
        SoftReset = 0x94,
        ActivateHeater_200mW_1000ms = 0x39,
        ActivateHeater_200mW_100ms = 0x32,
        ActivateHeater_110mW_1000ms = 0x2F,
        ActivateHeater_110mW_100ms = 0x24,
        ActivateHeater_20mW_1000ms = 0x1E,
        ActivateHeater_20mW_100ms = 0x15,
    };

    struct Params {
        //! How long to wait before receiving the operation result from the I2C device.
        TickType_t send_wait_interval { pdMS_TO_TICKS(10) };

        //! How long to wait for I2C operation to complete.
        core::Time bus_wait_interval { core::Duration::second * 5 };

        //! How precise data should be measured.
        Command measure_command { Command::MeasureHighPrecision };

        //! Heater activation command.
        Command heating_command { Command::ActivateHeater_20mW_100ms };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p transceiver to communicate with the I2C device.
    //!  - @p storage to persist number of times the heater was activated.
    Sensor(io::i2c::ITransceiver& transceiver, storage::IStorage& storage, Params params);

    //! Read sensor data.
    status::StatusCode run() override;

    //! Return the latest sensor data.
    Data get_data() const;

    //! Reset the sensor.
    //!
    //! @remarks
    //!  Should be called in the same context as run() call.
    status::StatusCode reset();

    //! Activate sensor internal heater.
    //!
    //! @notes
    //!  - Heating stops automatically.
    //!  - A high-precision measurement is done before the heater deactivation.
    //!
    //! @remarks
    //!  - Should be called in the same context as run() call.
    //!
    //!  - The heater is designed for a maximum duty cycle of 10%, meaning the total
    //!    heater-on-time should not be longer than 10% of the sensor’s lifetime.
    status::StatusCode heat();

private:
    static const char* command_to_str_(Command command);
    static TickType_t estimate_heating_delay_(Command command);

    static constexpr const char* heating_count_key_ = "heating_count";

    status::StatusCode reset_();
    status::StatusCode heat_();
    status::StatusCode read_serial_number_();
    status::StatusCode receive_data_(Data& data);
    status::StatusCode send_command_(Command command);
    status::StatusCode read_heating_count_();
    status::StatusCode write_heating_count_();

    // Long pulse timings:
    //  - 1100ms - heater-on duration.
    //  - 10ms - high precision measurement duration.
    //  - 50ms - for random delays.
    static const TickType_t heating_long_pulse_delay_ = pdMS_TO_TICKS(1100 + 10 + 50);

    // Short pulse timings:
    //  - 110ms - heater-on duration.
    //  - 10ms - high precision measurement duration.
    //  - 50ms - for random delays.
    static const TickType_t heating_short_pulse_delay_ = pdMS_TO_TICKS(110 + 10 + 50);

    const Params params_;

    io::i2c::ITransceiver& transceiver_;
    storage::IStorage& storage_;

    uint32_t serial_number_ { 0 };
    TickType_t heating_delay_ { 0 };

    core::SpmcNode<Data> data_;
    unsigned heating_count_ { 0 };
};

} // namespace sht41
} // namespace sensor
} // namespace ocs
