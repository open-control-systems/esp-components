/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/iclock.h"
#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/basic_counter_holder.h"
#include "ocs_io/adc_store.h"
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itimer.h"
#include "ocs_scheduler/timer_store.h"
#include "ocs_sensor/basic_sensor_task.h"
#include "ocs_sensor/yl69_sensor.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/fanout_reboot_handler.h"

namespace ocs {
namespace sensor {

//! Periodically read the soil moisture data.
class DefaultYL69SensorTask : public BasicSensorTask<YL69Sensor>,
                              public core::NonCopyable<> {
public:
    struct Params {
        YL69Sensor::Params sensor;
        core::microseconds_t read_interval { 0 };
    };

    //! Initialize.
    DefaultYL69SensorTask(core::IClock& clock,
                          io::AdcStore& adc_store,
                          storage::IStorage& storage,
                          system::FanoutRebootHandler& reboot_handler,
                          scheduler::AsyncTaskScheduler& task_scheduler,
                          scheduler::TimerStore& timer_store,
                          diagnostic::BasicCounterHolder& counter_holder,
                          Params params);
};

} // namespace sensor
} // namespace ocs
