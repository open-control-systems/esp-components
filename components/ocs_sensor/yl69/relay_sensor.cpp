/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/yl69/relay_sensor.h"
#include "ocs_io/default_gpio.h"
#include "ocs_io/delay_gpio.h"
#include "ocs_io/gpio_guard.h"

namespace ocs {
namespace sensor {
namespace yl69 {

RelaySensor::RelaySensor(scheduler::ITask& task,
                         gpio_num_t gpio,
                         TickType_t turn_on_delay_interval)
    : task_(task) {
    default_gpio_.reset(new (std::nothrow) io::DefaultGpio("relay_sensor", gpio));
    configASSERT(default_gpio_);

    delay_gpio_.reset(new (std::nothrow) io::DelayGpio(
        *default_gpio_,
        io::DelayGpio::Params {
            .flip_delay_interval = pdMS_TO_TICKS(0),
            .turn_on_delay_interval = turn_on_delay_interval,
            .turn_off_delay_interval = pdMS_TO_TICKS(0),
        }));
    configASSERT(delay_gpio_);

    gpio_ = delay_gpio_.get();
    configASSERT(gpio_);
}

status::StatusCode RelaySensor::run() {
    io::GpioGuard gpio(*gpio_);

    return task_.run();
}

} // namespace yl69
} // namespace sensor
} // namespace ocs
