/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_diagnostic/acc_persistent_counter.h"
#include "ocs_diagnostic/mem_persistent_counter.h"
#include "ocs_diagnostic/time_counter.h"
#include "ocs_pipeline/basic/system_counter_pipeline.h"
#include "ocs_storage/nvs_storage.h"

namespace ocs {
namespace pipeline {
namespace basic {

SystemCounterPipeline::SystemCounterPipeline(
    core::IClock& clock,
    storage::IStorage& storage,
    system::FanoutRebootHandler& reboot_handler,
    scheduler::ITaskScheduler& task_scheduler,
    diagnostic::BasicCounterHolder& counter_holder) {
    uptime_counter_.reset(new (std::nothrow) diagnostic::TimeCounter(
        clock, "c_sys_uptime", core::Duration::second));
    configASSERT(uptime_counter_);

    uptime_persistent_counter_.reset(
        new (std::nothrow) diagnostic::MemPersistentCounter(storage, *uptime_counter_));
    configASSERT(uptime_persistent_counter_);

    reboot_handler.add(*uptime_persistent_counter_);
    counter_holder.add(*uptime_persistent_counter_);

    lifetime_counter_.reset(new (std::nothrow) diagnostic::TimeCounter(
        clock, "c_sys_lifetime", core::Duration::second));
    configASSERT(lifetime_counter_);

    lifetime_persistent_counter_.reset(
        new (std::nothrow) diagnostic::AccPersistentCounter(storage, *lifetime_counter_));
    configASSERT(lifetime_persistent_counter_);

    configASSERT(task_scheduler.add(*lifetime_persistent_counter_,
                                    "lifetime_counter_task", core::Duration::hour)
                 == status::StatusCode::OK);

    reboot_handler.add(*lifetime_persistent_counter_);
    counter_holder.add(*lifetime_persistent_counter_);
}

} // namespace basic
} // namespace pipeline
} // namespace ocs
