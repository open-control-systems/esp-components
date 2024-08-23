/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_io/delay_gpio.h"

namespace ocs {
namespace io {

DelayGpio::DelayGpio(IGpio& gpio, DelayGpio::Params params)
    : params_(params)
    , gpio_(gpio) {
}

int DelayGpio::get() {
    return gpio_.get();
}

status::StatusCode DelayGpio::flip() {
    const auto code = gpio_.flip();
    if (code != status::StatusCode::OK) {
        return code;
    }

    if (params_.flip_delay_interval) {
        vTaskDelay(params_.flip_delay_interval);
    }

    return status::StatusCode::OK;
}

status::StatusCode DelayGpio::turn_on() {
    const auto code = gpio_.turn_on();
    if (code != status::StatusCode::OK) {
        return code;
    }

    if (params_.turn_on_delay_interval) {
        vTaskDelay(params_.turn_on_delay_interval);
    }

    return status::StatusCode::OK;
}

status::StatusCode DelayGpio::turn_off() {
    const auto code = gpio_.turn_off();
    if (code != status::StatusCode::OK) {
        return code;
    }

    if (params_.turn_off_delay_interval) {
        vTaskDelay(params_.turn_off_delay_interval);
    }

    return status::StatusCode::OK;
}

} // namespace io
} // namespace ocs
