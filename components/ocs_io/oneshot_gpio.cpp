/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_io/oneshot_gpio.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace io {

namespace {

const char* log_tag = "oneshot-gpio";

} // namespace

OneshotGpio::OneshotGpio(scheduler::ITask& task, IGpio& gpio)
    : task_(task)
    , gpio_(gpio) {
}

status::StatusCode OneshotGpio::run() {
    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_on());

    const auto code = task_.run();
    if (code != status::StatusCode::OK) {
        if (const auto c = gpio_.turn_off(); c != status::StatusCode::OK) {
            ocs_loge(log_tag, "failed to turn off GPIO on task failure: %s",
                     status::code_to_str(c));
        }
        return code;
    }

    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_off());

    return status::StatusCode::OK;
}

} // namespace io
} // namespace ocs
