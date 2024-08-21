/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/safe_yl69_sensor_task.h"
#include "ocs_scheduler/high_resolution_timer.h"
#include "ocs_sensor/relay_sensor.h"

namespace ocs {
namespace sensor {

SafeYL69SensorTask::SafeYL69SensorTask(core::IClock& clock,
                                       io::AdcStore& adc_store,
                                       storage::IStorage& storage,
                                       system::FanoutRebootHandler& reboot_handler,
                                       scheduler::AsyncTaskScheduler& task_scheduler,
                                       scheduler::TimerStore& timer_store,
                                       diagnostic::BasicCounterHolder& counter_holder,
                                       SafeYL69SensorTask::Params params) {
    sensor_.reset(new (std::nothrow) YL69Sensor(clock, adc_store, storage, reboot_handler,
                                                task_scheduler, timer_store,
                                                counter_holder, params.sensor));
    configASSERT(sensor_);

    relay_sensor_.reset(new (std::nothrow) RelaySensor(*sensor_, params.relay_gpio,
                                                       params.power_on_delay_interval));
    configASSERT(relay_sensor_);

    async_task_ = task_scheduler.add(*relay_sensor_);
    configASSERT(async_task_);

    async_task_timer_.reset(new (std::nothrow) scheduler::HighResolutionTimer(
        *async_task_, "Safe-YL69-sensor-control", params.read_interval));
    configASSERT(async_task_timer_);

    timer_store.add(*async_task_timer_);
}

} // namespace sensor
} // namespace ocs
