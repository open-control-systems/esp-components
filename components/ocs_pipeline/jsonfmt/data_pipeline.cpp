/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_pipeline/jsonfmt/data_pipeline.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

DataPipeline::DataPipeline(core::IClock& clock,
                           storage::StorageBuilder& storage_builder,
                           scheduler::ITaskScheduler& task_scheduler,
                           system::FanoutRebootHandler& reboot_handler,
                           RegistrationFormatter::Params registration_params) {
    telemetry_formatter_.reset(new (std::nothrow) TelemetryFormatter());
    configASSERT(telemetry_formatter_);

    registration_formatter_.reset(new (std::nothrow)
                                      RegistrationFormatter(registration_params));
    configASSERT(registration_formatter_);

    system_counter_storage_ = storage_builder.make("system_counter");
    configASSERT(system_counter_storage_);

    counter_formatter_.reset(new (std::nothrow) CounterFormatter());
    configASSERT(counter_formatter_);

    system_counter_pipeline_.reset(new (std::nothrow) basic::SystemCounterPipeline(
        clock, *system_counter_storage_, reboot_handler, task_scheduler,
        *counter_formatter_));
    configASSERT(system_counter_pipeline_);

    telemetry_formatter_->get_fanout_formatter().add(*counter_formatter_);
}

fmt::json::FanoutFormatter& DataPipeline::get_telemetry_formatter() {
    return telemetry_formatter_->get_fanout_formatter();
}

fmt::json::FanoutFormatter& DataPipeline::get_registration_formatter() {
    return registration_formatter_->get_fanout_formatter();
}

diagnostic::BasicCounterHolder& DataPipeline::get_counter_holder() {
    return *counter_formatter_;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
