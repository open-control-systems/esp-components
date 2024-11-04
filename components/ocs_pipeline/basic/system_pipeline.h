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
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_scheduler/idelay_estimator.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_status/code.h"
#include "ocs_storage/flash_initializer.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/fanout_reboot_handler.h"
#include "ocs_system/fanout_suspender.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace pipeline {
namespace basic {

class SystemPipeline : public core::NonCopyable<> {
public:
    struct Params {
        struct TaskScheduler {
            //! Delay after all tasks have been run.
            TickType_t delay { 0 };
        } task_scheduler;
    };

    //! Initialize.
    explicit SystemPipeline(Params params);

    //! Start the pipeline.
    //!
    //! @remarks
    //!  Blocking call.
    status::StatusCode start();

    core::IClock& get_clock();
    storage::StorageBuilder& get_storage_builder();
    scheduler::ITaskScheduler& get_task_scheduler();
    scheduler::ITask& get_reboot_task();
    system::FanoutRebootHandler& get_reboot_handler();
    system::FanoutSuspender& get_suspender();

private:
    std::unique_ptr<storage::FlashInitializer> flash_initializer_;
    std::unique_ptr<storage::StorageBuilder> storage_builder_;

    std::unique_ptr<core::IClock> default_clock_;

    std::unique_ptr<scheduler::IDelayEstimator> delay_estimator_;
    std::unique_ptr<scheduler::ITaskScheduler> task_scheduler_;
    std::unique_ptr<scheduler::AsyncFuncScheduler> func_scheduler_;

    std::unique_ptr<system::FanoutRebootHandler> fanout_reboot_handler_;
    std::unique_ptr<system::IRebooter> default_rebooter_;
    std::unique_ptr<system::IRebooter> delay_rebooter_;

    std::unique_ptr<scheduler::ITask> reboot_task_;
    std::unique_ptr<scheduler::ITask> reboot_task_async_;

    std::unique_ptr<system::FanoutSuspender> fanout_suspender_;
};

} // namespace basic
} // namespace pipeline
} // namespace ocs
