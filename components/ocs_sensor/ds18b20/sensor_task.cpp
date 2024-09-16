/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/ds18b20/sensor_task.h"
#include "ocs_io/default_gpio.h"
#include "ocs_scheduler/high_resolution_timer.h"
#include "ocs_system/delayer_configuration.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

SensorTask::SensorTask(scheduler::TimerStore& timer_store,
                       storage::IStorage& storage,
                       Store& sensor_store,
                       const char* sensor_id,
                       const char* timer_id,
                       SensorTask::Params params) {
    sensor_.reset(new (std::nothrow) Sensor(storage, sensor_id));
    configASSERT(sensor_);

    async_task_ = sensor_store.add(*sensor_, params.data_pin, "GPIO-DS18B20-onewire");
    configASSERT(async_task_);

    async_task_timer_.reset(new (std::nothrow) scheduler::HighResolutionTimer(
        *async_task_, timer_id, params.read_interval));
    configASSERT(async_task_timer_);

    timer_store.add(*async_task_timer_);
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
