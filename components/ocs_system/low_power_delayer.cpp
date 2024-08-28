/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "esp_attr.h"

#include "ocs_core/lock_guard.h"
#include "ocs_status/macros.h"
#include "ocs_system/low_power_delayer.h"

namespace ocs {
namespace system {

LowPowerDelayer::LowPowerDelayer(core::microseconds_t resolution)
    : BasicDelayer(resolution) {
    gptimer_event_callbacks_t cbs;
    cbs.on_alarm = handle_alarm_callback_;

    ESP_ERROR_CHECK(gptimer_register_event_callbacks(handle_, &cbs, this));
    ESP_ERROR_CHECK(gptimer_enable(handle_));
}

status::StatusCode LowPowerDelayer::delay(core::microseconds_t delay) {
    OCS_STATUS_RETURN_ON_FALSE(gptimer_set_raw_count(handle_, 0) == ESP_OK,
                               status::StatusCode::Error);

    gptimer_alarm_config_t config;
    config.alarm_count = delay;
    config.flags.auto_reload_on_alarm = false;

    OCS_STATUS_RETURN_ON_FALSE(gptimer_set_alarm_action(handle_, &config) == ESP_OK,
                               status::StatusCode::Error);

    OCS_STATUS_RETURN_ON_FALSE(gptimer_start(handle_) == ESP_OK,
                               status::StatusCode::Error);

    OCS_STATUS_RETURN_ON_ERROR(flag_.wait(portMAX_DELAY));

    OCS_STATUS_RETURN_ON_FALSE(gptimer_stop(handle_) == ESP_OK,
                               status::StatusCode::Error);

    return status::StatusCode::OK;
}

bool IRAM_ATTR LowPowerDelayer::handle_alarm_callback_(
    gptimer_handle_t handle, const gptimer_alarm_event_data_t* data, void* arg) {
    LowPowerDelayer& self = *static_cast<LowPowerDelayer*>(arg);

    return self.flag_.signal() == status::StatusCode::IsrYield;
}

} // namespace system
} // namespace ocs
