/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/system_pipeline.h"
#include "ocs_core/log.h"
#include "ocs_scheduler/high_resolution_timer.h"
#include "ocs_status/code_to_str.h"
#include "ocs_system/default_clock.h"
#include "ocs_system/default_rebooter.h"
#include "ocs_system/delay_rebooter.h"
#include "ocs_system/reboot_task.h"

namespace ocs {
namespace pipeline {

namespace {

const char* log_tag = "system-pipeline";

} // namespace

SystemPipeline::SystemPipeline() {
    flash_initializer_.reset(new (std::nothrow) storage::FlashInitializer());
    configASSERT(flash_initializer_);

    storage_builder_.reset(new (std::nothrow) storage::StorageBuilder());
    configASSERT(storage_builder_);

    default_clock_.reset(new (std::nothrow) system::DefaultClock());
    configASSERT(default_clock_);

    task_scheduler_.reset(new (std::nothrow) scheduler::AsyncTaskScheduler());
    configASSERT(task_scheduler_);

    timer_store_.reset(new (std::nothrow) scheduler::TimerStore());
    configASSERT(timer_store_);

    fanout_reboot_handler_.reset(new (std::nothrow) system::FanoutRebootHandler());
    configASSERT(fanout_reboot_handler_);

    default_rebooter_.reset(new (std::nothrow)
                                system::DefaultRebooter(*fanout_reboot_handler_));
    configASSERT(default_rebooter_);

    delay_rebooter_.reset(
        new (std::nothrow) system::DelayRebooter(pdMS_TO_TICKS(500), *default_rebooter_));
    configASSERT(delay_rebooter_);

    reboot_task_.reset(new (std::nothrow) system::RebootTask(*delay_rebooter_));
    configASSERT(reboot_task_);

    reboot_task_async_ = task_scheduler_->add(*reboot_task_);
    configASSERT(reboot_task_async_);
}

status::StatusCode SystemPipeline::start() {
    const auto code = timer_store_->start();
    if (code != status::StatusCode::OK) {
        return code;
    }

    ocs_logi(log_tag, "start handling tasks: count=%u max=%u", task_scheduler_->count(),
             scheduler::AsyncTaskScheduler::max_task_count);

    while (true) {
        const auto code = task_scheduler_->wait();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag, "failed to wait for asynchronous events: %s",
                     status::code_to_str(code));
        }
    }

    return status::StatusCode::OK;
}

core::IClock& SystemPipeline::get_clock() {
    return *default_clock_;
}

storage::StorageBuilder& SystemPipeline::get_storage_builder() {
    return *storage_builder_;
}

scheduler::AsyncTaskScheduler& SystemPipeline::get_task_scheduler() {
    return *task_scheduler_;
}

scheduler::TimerStore& SystemPipeline::get_timer_store() {
    return *timer_store_;
}

scheduler::ITask& SystemPipeline::get_reboot_task() {
    return *reboot_task_async_;
}

system::FanoutRebootHandler& SystemPipeline::get_reboot_handler() {
    return *fanout_reboot_handler_;
}

} // namespace pipeline
} // namespace ocs
