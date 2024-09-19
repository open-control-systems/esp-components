/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "driver/gpio.h"

#include "ocs_core/iclock.h"
#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/basic_counter_holder.h"
#include "ocs_io/adc_store.h"
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/timer_store.h"
#include "ocs_sensor/basic_sensor_task.h"
#include "ocs_sensor/yl69/sensor.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/fanout_reboot_handler.h"

namespace ocs {
namespace sensor {
namespace yl69 {

//! The sensor is only powered when the relay is activated.
class SafeSensorTask : public BasicSensorTask<Sensor>, public core::NonCopyable<> {
public:
    struct Params {
        Sensor::Params sensor;
        core::microseconds_t read_interval { 0 };
        gpio_num_t relay_gpio { GPIO_NUM_NC };
        TickType_t power_on_delay_interval { 0 };
    };

    //! Initialize.
    SafeSensorTask(core::IClock& clock,
                   io::AdcStore& adc_store,
                   storage::IStorage& storage,
                   system::FanoutRebootHandler& reboot_handler,
                   scheduler::AsyncTaskScheduler& task_scheduler,
                   scheduler::TimerStore& timer_store,
                   diagnostic::BasicCounterHolder& counter_holder,
                   const char* sensor_id,
                   const char* sensor_task_id,
                   const char* task_id,
                   Params params);

private:
    std::unique_ptr<scheduler::ITask> relay_sensor_;
};

} // namespace yl69
} // namespace sensor
} // namespace ocs
