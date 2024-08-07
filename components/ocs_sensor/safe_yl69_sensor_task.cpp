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
                                       diagnostic::BasicCounterHolder& counter_holder) {
    sensor_.reset(new (std::nothrow)
                      sensor::YL69Sensor(clock, adc_store, storage, reboot_handler,
                                         task_scheduler, timer_store, counter_holder));
    configASSERT(sensor_);

    relay_sensor_.reset(new (std::nothrow) sensor::RelaySensor(
        *sensor_, static_cast<gpio_num_t>(CONFIG_OCS_SENSOR_YL69_RELAY_GPIO),
        (1000 * CONFIG_OCS_SENSOR_YL69_POWER_ON_DELAY_INTERVAL) / portTICK_PERIOD_MS));
    configASSERT(relay_sensor_);

    async_task_ = task_scheduler.add(*relay_sensor_);
    configASSERT(async_task_);

    async_task_timer_.reset(new (std::nothrow) scheduler::HighResolutionTimer(
        *async_task_, "Safe-YL69-sensor-control",
        core::Second * CONFIG_OCS_SENSOR_YL69_READ_INTERVAL));
    configASSERT(async_task_timer_);

    timer_store.add(*async_task_timer_);
}

} // namespace sensor
} // namespace ocs
