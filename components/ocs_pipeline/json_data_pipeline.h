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
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_pipeline/counter_json_formatter.h"
#include "ocs_pipeline/registration_json_formatter.h"
#include "ocs_pipeline/system_counter_pipeline.h"
#include "ocs_pipeline/telemetry_json_formatter.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/fanout_reboot_handler.h"

namespace ocs {
namespace pipeline {

//! Component to handle telemetry and registration data.
class JsonDataPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    JsonDataPipeline(core::IClock& clock,
                     storage::StorageBuilder& storage_builder,
                     scheduler::ITaskScheduler& task_scheduler,
                     system::FanoutRebootHandler& reboot_handler,
                     RegistrationJsonFormatter::Params registration_params);

    fmt::json::FanoutFormatter& get_telemetry_formatter();
    fmt::json::FanoutFormatter& get_registration_formatter();
    diagnostic::BasicCounterHolder& get_counter_holder();

private:
    std::unique_ptr<TelemetryJsonFormatter> telemetry_formatter_;
    std::unique_ptr<RegistrationJsonFormatter> registration_formatter_;

    std::unique_ptr<storage::IStorage> system_counter_storage_;
    std::unique_ptr<CounterJsonFormatter> counter_json_formatter_;
    std::unique_ptr<SystemCounterPipeline> system_counter_pipeline_;
};

} // namespace pipeline
} // namespace ocs
