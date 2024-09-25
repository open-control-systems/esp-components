/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/ldr/sensor_task.h"

namespace ocs {
namespace sensor {
namespace ldr {

SensorTask::SensorTask(io::AdcStore& adc_store,
                       scheduler::ITaskScheduler& task_scheduler,
                       const char* sensor_id,
                       const char* task_id,
                       SensorTask::Params params) {
    sensor_.reset(new (std::nothrow) Sensor(adc_store, sensor_id, params.sensor));
    configASSERT(sensor_);

    configASSERT(task_scheduler.add(*sensor_, task_id, params.read_interval)
                 == status::StatusCode::OK);
}

Sensor& SensorTask::get_sensor() {
    return *sensor_;
}

} // namespace ldr
} // namespace sensor
} // namespace ocs
