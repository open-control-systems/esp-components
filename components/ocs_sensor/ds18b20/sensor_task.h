/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "driver/gpio.h"

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_scheduler/timer_store.h"
#include "ocs_sensor/basic_sensor_task.h"
#include "ocs_sensor/ds18b20/store.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

class SensorTask : public BasicSensorTask<Sensor>, public core::NonCopyable<> {
public:
    struct Params {
        core::microseconds_t read_interval { 0 };
        gpio_num_t data_pin { GPIO_NUM_NC };
    };

    //! Initialize.
    SensorTask(scheduler::TimerStore& timer_store,
               scheduler::AsyncTaskScheduler& task_scheduler,
               storage::IStorage& storage,
               Store& sensor_store,
               const char* sensor_id,
               const char* timer_id,
               Params params);
};

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
