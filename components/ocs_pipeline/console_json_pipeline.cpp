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

namespace ocs {
namespace pipeline {

ConsoleJsonPipeline::ConsoleJsonPipeline(scheduler::ITaskScheduler& task_scheduler,
                                         fmt::json::IFormatter& telemetry_formatter,
                                         fmt::json::IFormatter& registration_formatter,
                                         Params params) {
    telemetry_task_.reset(new (std::nothrow) ConsoleJsonTask(
        telemetry_formatter, "console-telemetry-task", params.telemetry.buffer_size));
    configASSERT(telemetry_task_);

    configASSERT(task_scheduler.add(*telemetry_task_, "console-telemetry",
                                    params.telemetry.interval)
                 == status::StatusCode::OK);

    registration_task_.reset(new (std::nothrow) ConsoleJsonTask(
        registration_formatter, "console-registration-task",
        params.registration.buffer_size));
    configASSERT(registration_task_);

    configASSERT(task_scheduler.add(*registration_task_, "console-registration",
                                    params.registration.interval)
                 == status::StatusCode::OK);
}

} // namespace pipeline
} // namespace ocs
