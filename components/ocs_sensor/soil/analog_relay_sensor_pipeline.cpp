/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/soil/analog_relay_sensor_pipeline.h"
#include "ocs_sensor/soil/analog_relay_sensor.h"
#include "ocs_sensor/soil/analog_sensor_pipeline.h"

namespace ocs {
namespace sensor {
namespace soil {

AnalogRelaySensorPipeline::AnalogRelaySensorPipeline(
    core::IClock& clock,
    io::AdcStore& adc_store,
    storage::StorageBuilder& storage_builder,
    system::FanoutRebootHandler& reboot_handler,
    scheduler::ITaskScheduler& task_scheduler,
    const char* id,
    AnalogRelaySensorPipeline::Params params)
    : task_id_(std::string(id) + "_task") {
    adc_ = adc_store.add(params.adc_channel);
    configASSERT(adc_);

    fsm_block_pipeline_.reset(new (std::nothrow) control::FsmBlockPipeline(
        clock, reboot_handler, task_scheduler, storage_builder, "soil_fsm",
        params.fsm_block));
    configASSERT(fsm_block_pipeline_);

    sensor_.reset(new (std::nothrow) AnalogSensor(*adc_, fsm_block_pipeline_->get_block(),
                                                  params.sensor));
    configASSERT(sensor_);

    relay_sensor_.reset(new (std::nothrow) AnalogRelaySensor(
        *sensor_, params.relay_gpio, params.power_on_delay_interval));
    configASSERT(relay_sensor_);

    configASSERT(
        task_scheduler.add(*relay_sensor_, task_id_.c_str(), params.read_interval)
        == status::StatusCode::OK);
}

AnalogSensor& AnalogRelaySensorPipeline::get_sensor() {
    return *sensor_;
}

} // namespace soil
} // namespace sensor
} // namespace ocs
