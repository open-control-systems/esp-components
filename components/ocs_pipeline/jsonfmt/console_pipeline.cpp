/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_pipeline/jsonfmt/console_pipeline.h"
#include "ocs_pipeline/jsonfmt/console_task.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

ConsolePipeline::ConsolePipeline(scheduler::ITaskScheduler& task_scheduler,
                                 fmt::json::IFormatter& telemetry_formatter,
                                 fmt::json::IFormatter& registration_formatter,
                                 Params params) {
    telemetry_task_.reset(new (std::nothrow) ConsoleTask(
        telemetry_formatter, "console_telemetry_task", params.telemetry.buffer_size));
    configASSERT(telemetry_task_);

    configASSERT(task_scheduler.add(*telemetry_task_, "console_telemetry",
                                    params.telemetry.interval)
                 == status::StatusCode::OK);

    registration_task_.reset(new (std::nothrow) ConsoleTask(
        registration_formatter, "console_registration_task",
        params.registration.buffer_size));
    configASSERT(registration_task_);

    configASSERT(task_scheduler.add(*registration_task_, "console_registration",
                                    params.registration.interval)
                 == status::StatusCode::OK);
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
