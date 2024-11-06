/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_control/fsm_block_pipeline.h"
#include "ocs_core/iclock.h"
#include "ocs_core/noncopyable.h"
#include "ocs_io/adc/istore.h"
#include "ocs_io/gpio/types.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/soil/analog_sensor.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/fanout_reboot_handler.h"

namespace ocs {
namespace sensor {
namespace soil {

//! The sensor is only powered when the relay is activated.
class AnalogRelaySensorPipeline : public core::NonCopyable<> {
public:
    struct Params {
        AnalogSensor::Params sensor;
        io::adc::Channel adc_channel { static_cast<io::adc::Channel>(0) };
        control::FsmBlockPipeline::Params fsm_block;
        core::Time read_interval { 0 };
        io::gpio::Gpio relay_gpio { static_cast<io::gpio::Gpio>(-1) };
        TickType_t power_on_delay_interval { 0 };
    };

    //! Initialize.
    AnalogRelaySensorPipeline(core::IClock& clock,
                              io::adc::IStore& adc_store,
                              storage::StorageBuilder& storage_builder,
                              system::FanoutRebootHandler& reboot_handler,
                              scheduler::ITaskScheduler& task_scheduler,
                              const char* id,
                              Params params);

    //! Return the underlying sensor.
    AnalogSensor& get_sensor();

private:
    const std::string task_id_;

    io::adc::IStore::IAdcPtr adc_;
    std::unique_ptr<control::FsmBlockPipeline> fsm_block_pipeline_;
    std::unique_ptr<AnalogSensor> sensor_;
    std::unique_ptr<scheduler::ITask> relay_sensor_;
};

} // namespace soil
} // namespace sensor
} // namespace ocs
