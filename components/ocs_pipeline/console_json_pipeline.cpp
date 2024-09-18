/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_pipeline/console_json_pipeline.h"
#include "ocs_pipeline/console_json_task.h"
#include "ocs_scheduler/high_resolution_timer.h"

namespace ocs {
namespace pipeline {

ConsoleJsonPipeline::ConsoleJsonPipeline(scheduler::AsyncTaskScheduler& task_scheduler,
                                         scheduler::TimerStore& timer_store,
                                         fmt::json::IFormatter& telemetry_formatter,
                                         fmt::json::IFormatter& registration_formatter,
                                         Params params) {
    telemetry_task_.reset(new (std::nothrow) ConsoleJsonTask(
        telemetry_formatter, "console-telemetry-task", params.telemetry.buffer_size));
    configASSERT(telemetry_task_);

    telemetry_task_async_ = task_scheduler.add(*telemetry_task_);
    configASSERT(telemetry_task_async_);

    telemetry_task_timer_.reset(new (std::nothrow) scheduler::HighResolutionTimer(
        *telemetry_task_async_, "console-telemetry", params.telemetry.interval));
    configASSERT(telemetry_task_timer_);

    timer_store.add(*telemetry_task_timer_);

    registration_task_.reset(new (std::nothrow) ConsoleJsonTask(
        registration_formatter, "console-registration-task",
        params.registration.buffer_size));
    configASSERT(registration_task_);

    registration_task_async_ = task_scheduler.add(*registration_task_);
    configASSERT(registration_task_async_);

    registration_task_timer_.reset(new (std::nothrow) scheduler::HighResolutionTimer(
        *registration_task_async_, "console-registration", params.registration.interval));
    configASSERT(registration_task_timer_);

    timer_store.add(*registration_task_timer_);
}

} // namespace pipeline
} // namespace ocs
