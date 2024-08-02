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
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_scheduler/timer_store.h"
#include "ocs_status/code.h"
#include "ocs_storage/flash_initializer.h"
#include "ocs_system/fanout_reboot_handler.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace iot {

class SystemPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    SystemPipeline();

    //! Start the pipeline.
    status::StatusCode start();

    core::IClock& get_clock();
    scheduler::AsyncTaskScheduler& get_task_scheduler();
    scheduler::TimerStore& get_timer_store();
    scheduler::ITask& get_reboot_task();
    system::FanoutRebootHandler& get_reboot_handler();

private:
    std::unique_ptr<storage::FlashInitializer> flash_initializer_;

    std::unique_ptr<core::IClock> default_clock_;

    std::unique_ptr<scheduler::AsyncTaskScheduler> task_scheduler_;
    std::unique_ptr<scheduler::TimerStore> timer_store_;

    std::unique_ptr<system::FanoutRebootHandler> fanout_reboot_handler_;
    std::unique_ptr<system::IRebooter> default_rebooter_;
    std::unique_ptr<system::IRebooter> delay_rebooter_;

    std::unique_ptr<scheduler::ITask> reboot_task_;
    scheduler::AsyncTaskScheduler::TaskPtr reboot_task_async_;
};

} // namespace iot
} // namespace ocs
