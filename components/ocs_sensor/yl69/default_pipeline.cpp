/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/yl69/default_pipeline.h"

namespace ocs {
namespace sensor {
namespace yl69 {

DefaultPipeline::DefaultPipeline(core::IClock& clock,
                                 io::AdcStore& adc_store,
                                 storage::StorageBuilder& storage_builder,
                                 system::FanoutRebootHandler& reboot_handler,
                                 scheduler::ITaskScheduler& task_scheduler,
                                 const char* id,
                                 DefaultPipeline::Params params)
    : sensor_id_(std::string(id) + "_sensor")
    , task_id_(std::string(id) + "_task") {
    fsm_block_pipeline_.reset(new (std::nothrow) control::FsmBlockPipeline(
        clock, reboot_handler, task_scheduler, storage_builder, "soil_fsm",
        params.fsm_block));
    configASSERT(fsm_block_pipeline_);

    sensor_.reset(new (std::nothrow) Sensor(adc_store, fsm_block_pipeline_->get_block(),
                                            sensor_id_.c_str(), params.sensor));
    configASSERT(sensor_);

    configASSERT(task_scheduler.add(*sensor_, task_id_.c_str(), params.read_interval)
                 == status::StatusCode::OK);
}

Sensor& DefaultPipeline::get_sensor() {
    return *sensor_;
}

} // namespace yl69
} // namespace sensor
} // namespace ocs
