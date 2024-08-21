/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_core/time.h"
#include "ocs_io/adc_store.h"
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_scheduler/timer_store.h"
#include "ocs_sensor/basic_sensor_task.h"
#include "ocs_sensor/ldr_sensor.h"

namespace ocs {
namespace sensor {

class LdrSensorTask : public BasicSensorTask<LdrSensor>, public core::NonCopyable<> {
public:
    struct Params {
        LdrSensor::Params sensor;
        core::microseconds_t read_interval { 0 };
    };

    //! Initialize.
    LdrSensorTask(io::AdcStore& adc_store,
                  scheduler::AsyncTaskScheduler& task_scheduler,
                  scheduler::TimerStore& timer_store,
                  Params params);
};

} // namespace sensor
} // namespace ocs
