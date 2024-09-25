/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_core/time.h"
#include "ocs_fmt/json/iformatter.h"
#include "ocs_scheduler/itask_scheduler.h"

namespace ocs {
namespace pipeline {

//! Component to print telemetry and registration data to the console.
class ConsoleJsonPipeline : public core::NonCopyable<> {
public:
    struct DataParams {
        //! How often print data to the console.
        core::microseconds_t interval { 0 };

        //! Buffer size to hold the formatted JSON data, in bytes.
        unsigned buffer_size { 0 };
    };

    struct Params {
        DataParams telemetry;
        DataParams registration;
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p telemetry_formatter to format telemetry data.
    //!  - @p registration_formatter to format registration data.
    ConsoleJsonPipeline(scheduler::ITaskScheduler& task_scheduler,
                        fmt::json::IFormatter& telemetry_formatter,
                        fmt::json::IFormatter& registration_formatter,
                        Params params);

private:
    std::unique_ptr<scheduler::ITask> telemetry_task_;
    std::unique_ptr<scheduler::ITask> registration_task_;
};

} // namespace pipeline
} // namespace ocs
