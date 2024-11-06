/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_io/gpio/gpio_guard.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace io {
namespace gpio {

namespace {

const char* log_tag = "gpio_guard";

} // namespace

GpioGuard::GpioGuard(IGpio& gpio)
    : gpio_(gpio) {
    const auto code = gpio_.turn_on();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to turn on: %s", status::code_to_str(code));
    }
}

GpioGuard::~GpioGuard() {
    const auto code = gpio_.turn_off();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to turn off: %s", status::code_to_str(code));
    }
}

} // namespace gpio
} // namespace io
} // namespace ocs
