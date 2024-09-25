/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/yl69/default_sensor_task.h"

namespace ocs {
namespace sensor {
namespace yl69 {

DefaultSensorTask::DefaultSensorTask(core::IClock& clock,
                                     io::AdcStore& adc_store,
                                     storage::IStorage& storage,
                                     system::FanoutRebootHandler& reboot_handler,
                                     scheduler::ITaskScheduler& task_scheduler,
                                     diagnostic::BasicCounterHolder& counter_holder,
                                     const char* sensor_id,
                                     const char* sensor_task_id,
                                     const char* task_id,
                                     DefaultSensorTask::Params params) {
    sensor_.reset(new (std::nothrow)
                      Sensor(clock, adc_store, storage, reboot_handler, task_scheduler,
                             counter_holder, sensor_id, sensor_task_id, params.sensor));
    configASSERT(sensor_);

    configASSERT(task_scheduler.add(*sensor_, task_id, params.read_interval)
                 == status::StatusCode::OK);
}

Sensor& DefaultSensorTask::get_sensor() {
    return *sensor_;
}

} // namespace yl69
} // namespace sensor
} // namespace ocs
