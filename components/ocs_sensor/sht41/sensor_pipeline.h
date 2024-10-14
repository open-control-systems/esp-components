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
#include "ocs_i2c/istore.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/sht41/sensor.h"

namespace ocs {
namespace sensor {
namespace sht41 {

class SensorPipeline : public core::NonCopyable<> {
public:
    struct Params {
        core::microseconds_t read_interval { 0 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p store to register an I2C device.
    //!  - @p task_scheduler to register a task for periodic sensor reading.
    SensorPipeline(i2c::IStore& store,
                   scheduler::ITaskScheduler& task_scheduler,
                   Params params);

    //! Return the underlying sensor.
    Sensor& get_sensor();

private:
    i2c::IStore::ITransceiverPtr transceiver_;
    std::unique_ptr<Sensor> sensor_;
};

} // namespace sht41
} // namespace sensor
} // namespace ocs
