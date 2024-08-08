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
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itimer.h"
#include "ocs_scheduler/timer_store.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/fanout_reboot_handler.h"

namespace ocs {
namespace sensor {

class YL69Sensor : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Known soil statuses.
    enum class SoilStatus {
        None,
        Dry,
        Wet,
        Last,
    };

    //! Convert soil moisture status to human-readable description.
    static const char* soil_status_to_str(SoilStatus);

    //! Various sensor characteristics.
    struct Data {
        int raw { 0 };
        int voltage { 0 };
        int moisture { 0 };
        SoilStatus status { SoilStatus::None };
    };

    //! Initialize.
    YL69Sensor(core::IClock& clock,
               io::AdcStore& adc_store,
               storage::IStorage& storage,
               system::FanoutRebootHandler& reboot_handler,
               scheduler::AsyncTaskScheduler& task_scheduler,
               scheduler::TimerStore& timer_store,
               diagnostic::BasicCounterHolder& counter_holder);

    //! Read soil moisture data.
    status::StatusCode run() override;

    //! Return the underlying sensor data.
    Data get_data() const;

private:
    int calculate_moisture_(int raw) const;

    void update_data_(int raw, int voltage, SoilStatus status);

    const int threshold_dry_ { 0 };
    const int threshold_wet_ { 0 };

    io::IAdc* adc_ { nullptr };

    std::unique_ptr<diagnostic::StateCounter> dry_state_task_;
    std::unique_ptr<diagnostic::StateCounter> wet_state_task_;

    std::unique_ptr<scheduler::FanoutTask> fanout_task_;
    scheduler::ITask* fanout_task_async_ { nullptr };
    std::unique_ptr<scheduler::ITimer> task_timer_;

    std::atomic<Data> data_;
};

} // namespace sensor
} // namespace ocs
