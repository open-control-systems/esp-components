/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "esp_log.h"

#include "freertos/FreeRTOSConfig.h"

#include "ocs_scheduler/high_resolution_timer.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace scheduler {

namespace {

const char* log_tag = "high-resolution-timer";

} // namespace

HighResolutionTimer::HighResolutionTimer(ITask& task,
                                         const char* name,
                                         core::microseconds_t interval)
    : interval_(interval)
    , task_(task) {
    configASSERT(name);
    configASSERT(interval_);

    esp_timer_create_args_t args;
    memset(&args, 0, sizeof(args));

    args.callback = handle_cb_;
    args.arg = this;
    args.dispatch_method = ESP_TIMER_TASK;
    args.name = name;

    ESP_ERROR_CHECK(esp_timer_create(&args, &handle_));
}

HighResolutionTimer::~HighResolutionTimer() {
    configASSERT(handle_);
    ESP_ERROR_CHECK(esp_timer_delete(handle_));
}

status::StatusCode HighResolutionTimer::start() {
    const auto err = esp_timer_start_periodic(handle_, interval_);
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "esp_timer_start_periodic(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode HighResolutionTimer::stop() {
    const auto err = esp_timer_stop(handle_);
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "esp_timer_stop(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

void HighResolutionTimer::handle_cb_(void* arg) {
    configASSERT(arg);

    HighResolutionTimer& self = *static_cast<HighResolutionTimer*>(arg);
    const auto code = self.task_.run();
    ESP_LOGD(log_tag, "failed to run task: code=%s", status::code_to_str(code));
}

} // namespace scheduler
} // namespace ocs
