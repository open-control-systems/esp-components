/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <atomic>
#include <memory>

#include "ocs_core/iclock.h"
#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/basic_counter_holder.h"
#include "ocs_diagnostic/icounter.h"
#include "ocs_diagnostic/state_counter.h"
#include "ocs_io/adc_store.h"
#include "ocs_io/iadc.h"
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_scheduler/fanout_task.h"
#include "ocs_scheduler/itimer.h"
#include "ocs_scheduler/timer_store.h"
#include "ocs_sensor/basic_sensor.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/fanout_reboot_handler.h"

namespace ocs {
namespace sensor {
namespace yl69 {

//! Various sensor characteristics.
struct SensorData {
    //! Known soil statuses.
    enum class SoilStatus {
        None,
        Dry,
        Wet,
        Last,
    };

    //! Convert soil moisture status to human-readable description.
    static const char* soil_status_to_str(SoilStatus);

    int raw { 0 };
    int voltage { 0 };
    int moisture { 0 };
    SoilStatus status { SoilStatus::None };
};

class Sensor : public BasicSensor<SensorData>, public core::NonCopyable<> {
public:
    struct Params {
        unsigned value_min { 0 };
        unsigned value_max { 0 };
        adc_channel_t adc_channel { ADC_CHANNEL_0 };
    };

    //! Initialize.
    Sensor(core::IClock& clock,
           io::AdcStore& adc_store,
           storage::IStorage& storage,
           system::FanoutRebootHandler& reboot_handler,
           scheduler::AsyncTaskScheduler& task_scheduler,
           scheduler::TimerStore& timer_store,
           diagnostic::BasicCounterHolder& counter_holder,
           const char* sensor_id,
           const char* task_timer_id,
           Params params);

    //! Read sensor data.
    status::StatusCode run() override;

private:
    int calculate_moisture_(int raw) const;
    SensorData::SoilStatus calculate_status_(int raw) const;
    SensorData::SoilStatus update_data_(int raw, int voltage);

    const Params params_;

    io::IAdc* adc_ { nullptr };

    std::unique_ptr<diagnostic::StateCounter> dry_state_task_;
    std::unique_ptr<diagnostic::StateCounter> wet_state_task_;

    std::unique_ptr<scheduler::FanoutTask> fanout_task_;
    scheduler::ITask* fanout_task_async_ { nullptr };
    std::unique_ptr<scheduler::ITimer> task_timer_;
};

} // namespace yl69
} // namespace sensor
} // namespace ocs
