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
#include "ocs_io/igpio.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace sensor {
namespace yl69 {

//! Activate the sensor only when the relay is energized.
class RelaySensor : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p task to perform actual sensor reading.
    //!  - @p gpio - relay GPIO.
    //!  - @p turn_on_delay_interval - how long to wait after the relay is activated.
    RelaySensor(scheduler::ITask& task,
                gpio_num_t gpio,
                TickType_t turn_on_delay_interval);

    //! Energize the relay, run the underlying task, de-energized the relay.
    status::StatusCode run() override;

private:
    scheduler::ITask& task_;

    std::unique_ptr<io::IGpio> default_gpio_;
    std::unique_ptr<io::IGpio> delay_gpio_;

    io::IGpio* gpio_ { nullptr };
};

} // namespace yl69
} // namespace sensor
} // namespace ocs
