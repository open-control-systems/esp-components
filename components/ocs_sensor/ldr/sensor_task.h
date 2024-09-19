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
#include "ocs_sensor/ldr/sensor.h"

namespace ocs {
namespace sensor {
namespace ldr {

class SensorTask : public BasicSensorTask<Sensor>, public core::NonCopyable<> {
public:
    struct Params {
        Sensor::Params sensor;
        core::microseconds_t read_interval { 0 };
    };

    //! Initialize.
    SensorTask(io::AdcStore& adc_store,
               scheduler::AsyncTaskScheduler& task_scheduler,
               scheduler::TimerStore& timer_store,
               const char* sensor_id,
               const char* task_timer_id,
               Params params);
};

} // namespace ldr
} // namespace sensor
} // namespace ocs
