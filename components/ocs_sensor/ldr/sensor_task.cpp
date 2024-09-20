/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/ldr/sensor_task.h"
#include "ocs_scheduler/high_resolution_timer.h"

namespace ocs {
namespace sensor {
namespace ldr {

SensorTask::SensorTask(io::AdcStore& adc_store,
                       scheduler::AsyncTaskScheduler& task_scheduler,
                       scheduler::TimerStore& timer_store,
                       const char* sensor_id,
                       const char* task_timer_id,
                       SensorTask::Params params) {
    sensor_.reset(new (std::nothrow) Sensor(adc_store, sensor_id, params.sensor));
    configASSERT(sensor_);

    async_task_ = task_scheduler.add(*sensor_, task_timer_id);
    configASSERT(async_task_);

    async_task_timer_.reset(new (std::nothrow) scheduler::HighResolutionTimer(
        *async_task_, task_timer_id, params.read_interval));
    configASSERT(async_task_timer_);

    timer_store.add(*async_task_timer_);
}

} // namespace ldr
} // namespace sensor
} // namespace ocs
