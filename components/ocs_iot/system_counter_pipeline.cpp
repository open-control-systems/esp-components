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
#include "ocs_storage/ns_storage.h"

namespace ocs {
namespace iot {

SystemCounterPipeline::SystemCounterPipeline(
    core::IClock& clock,
    storage::StorageBuilder& storage_builder,
    system::FanoutRebootHandler& reboot_handler,
    diagnostic::BasicCounterHolder& counter_holder) {
    storage_ = storage_builder.make("system_counter");
    configASSERT(storage_);

    uptime_counter_.reset(
        new (std::nothrow) diagnostic::TimeCounter(clock, "c_sys_uptime", core::Second));

    uptime_persistent_counter_.reset(
        new (std::nothrow) diagnostic::LiveCounter(*storage_, *uptime_counter_));

    reboot_handler.add(*uptime_persistent_counter_);
    counter_holder.add(*uptime_persistent_counter_);

    lifetime_counter_.reset(new (std::nothrow) diagnostic::TimeCounter(
        clock, "c_sys_lifetime", core::Second));

    lifetime_persistent_counter_.reset(
        new (std::nothrow) diagnostic::PersistentCounter(*storage_, *lifetime_counter_));

    reboot_handler.add(*lifetime_persistent_counter_);
    counter_holder.add(*lifetime_persistent_counter_);
}

} // namespace iot
} // namespace ocs
