/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/ds18b20/sensor_task.h"
#include "ocs_io/default_gpio.h"
#include "ocs_scheduler/operation_guard_task.h"
#include "ocs_system/delayer_configuration.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

SensorTask::SensorTask(scheduler::ITaskScheduler& task_scheduler,
                       storage::IStorage& storage,
                       Store& sensor_store,
                       const char* sensor_id,
                       const char* task_id,
                       SensorTask::Params params) {
    sensor_.reset(new (std::nothrow) Sensor(storage, sensor_id));
    configASSERT(sensor_);

    sensor_task_.reset(new (std::nothrow) scheduler::OperationGuardTask(*sensor_));
    configASSERT(sensor_task_);

    configASSERT(sensor_store.add(*sensor_, params.data_pin, "GPIO-DS18B20-onewire")
                 == status::StatusCode::OK);

    configASSERT(task_scheduler.add(*sensor_task_, task_id, params.read_interval)
                 == status::StatusCode::OK);
}

Sensor& SensorTask::get_sensor() {
    return *sensor_;
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
