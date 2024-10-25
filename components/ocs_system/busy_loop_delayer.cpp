/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_status/macros.h"
#include "ocs_system/busy_loop_delayer.h"

namespace ocs {
namespace system {

BusyLoopDelayer::BusyLoopDelayer(core::Time resolution)
    : BasicDelayer(resolution) {
    ESP_ERROR_CHECK(gptimer_enable(handle_));
}

status::StatusCode BusyLoopDelayer::delay(core::Time delay) {
    OCS_STATUS_RETURN_ON_FALSE(gptimer_set_raw_count(handle_, 0) == ESP_OK,
                               status::StatusCode::Error);

    OCS_STATUS_RETURN_ON_FALSE(gptimer_start(handle_) == ESP_OK,
                               status::StatusCode::Error);

    while (1) {
        uint64_t value = 0;

        OCS_STATUS_RETURN_ON_FALSE(gptimer_get_raw_count(handle_, &value) == ESP_OK,
                                   status::StatusCode::Error);

        if (value >= delay) {
            break;
        }
    }

    OCS_STATUS_RETURN_ON_FALSE(gptimer_stop(handle_) == ESP_OK,
                               status::StatusCode::Error);

    return status::StatusCode::OK;
}

} // namespace system
} // namespace ocs
