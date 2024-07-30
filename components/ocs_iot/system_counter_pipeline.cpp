/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_diagnostic/live_counter.h"
#include "ocs_diagnostic/time_counter.h"
#include "ocs_iot/system_counter_pipeline.h"
#include "ocs_scheduler/high_resolution_timer.h"
#include "ocs_storage/ns_storage.h"

namespace ocs {
namespace iot {

SystemCounterPipeline::SystemCounterPipeline(
    core::IClock& clock,
    storage::IStorage& storage,
    system::FanoutRebootHandler& reboot_handler,
    scheduler::AsyncTaskScheduler& task_scheduler,
    scheduler::TimerStore& timer_store,
    diagnostic::BasicCounterHolder& counter_holder) {
    uptime_counter_.reset(
        new (std::nothrow) diagnostic::TimeCounter(clock, "c_sys_uptime", core::Second));
    configASSERT(uptime_counter_);

    uptime_persistent_counter_.reset(
        new (std::nothrow) diagnostic::LiveCounter(storage, *uptime_counter_));
    configASSERT(uptime_persistent_counter_);

    reboot_handler.add(*uptime_persistent_counter_);
    counter_holder.add(*uptime_persistent_counter_);

    lifetime_counter_.reset(new (std::nothrow) diagnostic::TimeCounter(
        clock, "c_sys_lifetime", core::Second));
    configASSERT(lifetime_counter_);

    lifetime_persistent_counter_.reset(
        new (std::nothrow) diagnostic::PersistentCounter(storage, *lifetime_counter_));
    configASSERT(lifetime_persistent_counter_);

    lifetime_counter_task_async_ = task_scheduler.add(*lifetime_persistent_counter_);
    configASSERT(lifetime_counter_task_async_);

    lifetime_counter_timer_.reset(new (std::nothrow) scheduler::HighResolutionTimer(
        *lifetime_counter_task_async_, lifetime_persistent_counter_->id(), core::Hour));
    configASSERT(lifetime_counter_timer_);

    timer_store.add(*lifetime_counter_timer_);

    reboot_handler.add(*lifetime_persistent_counter_);
    counter_holder.add(*lifetime_persistent_counter_);
}

} // namespace iot
} // namespace ocs
