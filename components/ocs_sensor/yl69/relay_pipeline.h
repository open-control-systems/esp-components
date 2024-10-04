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

#include "driver/gpio.h"

#include "ocs_control/fsm_block_pipeline.h"
#include "ocs_core/iclock.h"
#include "ocs_core/noncopyable.h"
#include "ocs_io/adc_store.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/yl69/sensor.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/fanout_reboot_handler.h"

namespace ocs {
namespace sensor {
namespace yl69 {

//! The sensor is only powered when the relay is activated.
class RelayPipeline : public core::NonCopyable<> {
public:
    struct Params {
        Sensor::Params sensor;
        core::microseconds_t read_interval { 0 };
        gpio_num_t relay_gpio { GPIO_NUM_NC };
        TickType_t power_on_delay_interval { 0 };
    };

    //! Initialize.
    RelayPipeline(core::IClock& clock,
                  io::AdcStore& adc_store,
                  storage::StorageBuilder& storage_builder,
                  system::FanoutRebootHandler& reboot_handler,
                  scheduler::ITaskScheduler& task_scheduler,
                  const char* id,
                  Params params);

    //! Return the underlying sensor.
    Sensor& get_sensor();

private:
    const std::string sensor_id_;
    const std::string task_id_;

    std::unique_ptr<control::FsmBlockPipeline> fsm_block_pipeline_;
    std::unique_ptr<Sensor> sensor_;
    std::unique_ptr<scheduler::ITask> relay_sensor_;
};

} // namespace yl69
} // namespace sensor
} // namespace ocs
