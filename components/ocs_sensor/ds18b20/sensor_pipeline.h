/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "driver/gpio.h"

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/ds18b20/store.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

class SensorPipeline : public core::NonCopyable<> {
public:
    struct Params {
        core::microseconds_t read_interval { 0 };
        gpio_num_t data_pin { GPIO_NUM_NC };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p task_scheduler to schedule periodic sensor readings.
    //!  - @p storage to persist sensor configuration.
    //!  - @p sensor_store to configure sensor.
    //!  - @p id to distinguish one sensor from another.
    SensorPipeline(scheduler::ITaskScheduler& task_scheduler,
                   storage::IStorage& storage,
                   Store& sensor_store,
                   const char* id,
                   Params params);

    //! Return the underlying sensor.
    Sensor& get_sensor();

private:
    const std::string task_id_;

    std::unique_ptr<Sensor> sensor_;
    std::unique_ptr<scheduler::ITask> sensor_task_;
};

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
