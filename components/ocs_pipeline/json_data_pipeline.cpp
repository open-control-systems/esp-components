/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/json_data_pipeline.h"

namespace ocs {
namespace pipeline {

JsonDataPipeline::JsonDataPipeline(
    core::IClock& clock,
    storage::StorageBuilder& storage_builder,
    scheduler::AsyncTaskScheduler& task_scheduler,
    scheduler::TimerStore& timer_store,
    system::FanoutRebootHandler& reboot_handler,
    RegistrationJsonFormatter::Params registration_params) {
    telemetry_formatter_.reset(new (std::nothrow) TelemetryJsonFormatter());
    configASSERT(telemetry_formatter_);

    registration_formatter_.reset(new (std::nothrow)
                                      RegistrationJsonFormatter(registration_params));
    configASSERT(registration_formatter_);

    system_counter_storage_ = storage_builder.make("system_counter");
    configASSERT(system_counter_storage_);

    counter_json_formatter_.reset(new (std::nothrow) CounterJsonFormatter());
    configASSERT(counter_json_formatter_);

    system_counter_pipeline_.reset(new (std::nothrow) SystemCounterPipeline(
        clock, *system_counter_storage_, reboot_handler, task_scheduler, timer_store,
        *counter_json_formatter_));
    configASSERT(system_counter_pipeline_);

    telemetry_formatter_->get_fanout_formatter().add(*counter_json_formatter_);
}

fmt::json::FanoutFormatter& JsonDataPipeline::get_telemetry_formatter() {
    return telemetry_formatter_->get_fanout_formatter();
}

fmt::json::FanoutFormatter& JsonDataPipeline::get_registration_formatter() {
    return registration_formatter_->get_fanout_formatter();
}

diagnostic::BasicCounterHolder& JsonDataPipeline::get_counter_holder() {
    return *counter_json_formatter_;
}

} // namespace pipeline
} // namespace ocs
