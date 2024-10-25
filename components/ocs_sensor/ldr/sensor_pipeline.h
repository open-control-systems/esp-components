/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_core/time.h"
#include "ocs_io/adc_store.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/ldr/sensor.h"

namespace ocs {
namespace sensor {
namespace ldr {

class SensorPipeline : public core::NonCopyable<> {
public:
    struct Params {
        Sensor::Params sensor;
        core::Time read_interval { 0 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p adc_store to perform ADC readings.
    //!  - @p task_scheduler to schedule periodic ADC readings.
    //!  - @p id to distringuish one sensor from another.
    SensorPipeline(io::AdcStore& adc_store,
                   scheduler::ITaskScheduler& task_scheduler,
                   const char* id,
                   Params params);

    //! Return the underlying sensor.
    Sensor& get_sensor();

private:
    const std::string sensor_id_;
    const std::string task_id_;

    std::unique_ptr<Sensor> sensor_;
};

} // namespace ldr
} // namespace sensor
} // namespace ocs
