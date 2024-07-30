/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "esp_log.h"

#include "freertos/FreeRTOSConfig.h"

#include "ocs_scheduler/timer_store.h"

namespace ocs {
namespace scheduler {

namespace {

const char* log_tag = "timer-store";

} // namespace

void TimerStore::add(ITimer& timer) {
    timers_.emplace_back(&timer);
}

status::StatusCode TimerStore::start() {
    if (!timers_.size()) {
        return status::StatusCode::Error;
    }

    ESP_LOGI(log_tag, "start timers: count=%u", timers_.size());

    for (auto& timer : timers_) {
        const auto code = timer->start();
        if (code != status::StatusCode::OK) {
            return code;
        }
    }

    return status::StatusCode::OK;
}

} // namespace scheduler
} // namespace ocs
