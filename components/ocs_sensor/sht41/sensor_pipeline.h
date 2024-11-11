/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_core/time.h"
#include "ocs_io/i2c/istore.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/sht41/sensor.h"

namespace ocs {
namespace sensor {
namespace sht41 {

class SensorPipeline : public core::NonCopyable<> {
public:
    struct Params {
        core::Time read_interval { 0 };
        Sensor::Command measure_command { Sensor::Command::MeasureHighPrecision };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p store to register an I2C device.
    //!  - @p task_scheduler to register a task for periodic sensor reading.
    SensorPipeline(io::i2c::IStore& store,
                   scheduler::ITaskScheduler& task_scheduler,
                   Params params);

    //! Return the underlying sensor.
    Sensor& get_sensor();

private:
    io::i2c::IStore::ITransceiverPtr transceiver_;
    std::unique_ptr<Sensor> sensor_;
};

} // namespace sht41
} // namespace sensor
} // namespace ocs
