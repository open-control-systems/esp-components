/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/noncopyable.h"
#include "ocs_iot/console_json_task.h"
#include "ocs_iot/ijson_formatter.h"
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_scheduler/high_resolution_timer.h"
#include "ocs_scheduler/timer_store.h"

namespace ocs {
namespace iot {

//! Component to print telemetry and registration data to the console.
template <unsigned TelemetrySize, unsigned RegistrationSize>
class ConsoleJsonPipeline : public core::NonCopyable<> {
public:
    struct Params {
        //! How often print telemetry data to the console.
        core::microseconds_t telemetry_interval { core::Second * 10 };

        //! How often print registration data to the console.
        core::microseconds_t registration_interval { core::Second * 30 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p telemetry_formatter to format telemetry data.
    //!  - @p registration_formatter to format registration data.
    ConsoleJsonPipeline(scheduler::AsyncTaskScheduler& task_scheduler,
                        scheduler::TimerStore& timer_store,
                        IJsonFormatter& telemetry_formatter,
                        IJsonFormatter& registration_formatter,
                        Params params) {
        telemetry_task_.reset(new (std::nothrow) TelemetryTask("console-telemetry-task",
                                                               telemetry_formatter));
        configASSERT(telemetry_task_);

        telemetry_task_async_ = task_scheduler.add(*telemetry_task_);
        configASSERT(telemetry_task_async_);

        telemetry_task_timer_.reset(new (std::nothrow) scheduler::HighResolutionTimer(
            *telemetry_task_async_, "console-telemetry", params.telemetry_interval));
        configASSERT(telemetry_task_timer_);

        timer_store.add(*telemetry_task_timer_);

        registration_task_.reset(new (std::nothrow) RegistrationTask(
            "console-registration-task", registration_formatter));
        configASSERT(registration_task_);

        registration_task_async_ = task_scheduler.add(*registration_task_);
        configASSERT(registration_task_async_);

        registration_task_timer_.reset(new (std::nothrow) scheduler::HighResolutionTimer(
            *registration_task_async_, "console-registration",
            params.registration_interval));
        configASSERT(registration_task_timer_);

        timer_store.add(*registration_task_timer_);
    }

private:
    using TelemetryTask = ConsoleJsonTask<TelemetrySize>;
    using RegistrationTask = ConsoleJsonTask<RegistrationSize>;

    std::unique_ptr<scheduler::ITask> telemetry_task_;
    scheduler::ITask* telemetry_task_async_ { nullptr };
    std::unique_ptr<scheduler::ITimer> telemetry_task_timer_;

    std::unique_ptr<scheduler::ITask> registration_task_;
    scheduler::ITask* registration_task_async_ { nullptr };
    std::unique_ptr<scheduler::ITimer> registration_task_timer_;
};

} // namespace iot
} // namespace ocs
