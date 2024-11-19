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
#include "ocs_pipeline/basic/system_counter_pipeline.h"
#include "ocs_pipeline/jsonfmt/counter_formatter.h"
#include "ocs_pipeline/jsonfmt/registration_formatter.h"
#include "ocs_pipeline/jsonfmt/telemetry_formatter.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/fanout_reboot_handler.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

//! Component to handle telemetry and registration data.
class DataPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    DataPipeline(core::IClock& clock,
                 storage::StorageBuilder& storage_builder,
                 scheduler::ITaskScheduler& task_scheduler,
                 system::FanoutRebootHandler& reboot_handler,
                 const system::DeviceInfo& device_info);

    fmt::json::FanoutFormatter& get_telemetry_formatter();
    fmt::json::FanoutFormatter& get_registration_formatter();
    diagnostic::BasicCounterHolder& get_counter_holder();

private:
    std::unique_ptr<TelemetryFormatter> telemetry_formatter_;
    std::unique_ptr<RegistrationFormatter> registration_formatter_;

    std::unique_ptr<storage::IStorage> system_counter_storage_;
    std::unique_ptr<CounterFormatter> counter_formatter_;
    std::unique_ptr<basic::SystemCounterPipeline> system_counter_pipeline_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
