/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/yl69/default_pipeline.h"

namespace ocs {
namespace sensor {
namespace yl69 {

DefaultPipeline::DefaultPipeline(core::IClock& clock,
                                 io::AdcStore& adc_store,
                                 storage::StorageBuilder& storage_builder,
                                 system::FanoutRebootHandler& reboot_handler,
                                 scheduler::ITaskScheduler& task_scheduler,
                                 const char* sensor_id,
                                 const char* task_id,
                                 DefaultPipeline::Params params) {
    fsm_block_pipeline_.reset(new (std::nothrow) control::FsmBlockPipeline(
        clock, reboot_handler, task_scheduler, storage_builder, "soil-fsm",
        control::FsmBlockPipeline::Params {
            .state_save_interval = core::Minute * 30,
            .state_interval_resolution = core::Second,
        }));
    configASSERT(fsm_block_pipeline_);

    sensor_.reset(new (std::nothrow) Sensor(adc_store, fsm_block_pipeline_->get_block(),
                                            sensor_id, params.sensor));
    configASSERT(sensor_);

    configASSERT(task_scheduler.add(*sensor_, task_id, params.read_interval)
                 == status::StatusCode::OK);
}

Sensor& DefaultPipeline::get_sensor() {
    return *sensor_;
}

} // namespace yl69
} // namespace sensor
} // namespace ocs
