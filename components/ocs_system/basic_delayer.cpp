/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "esp_err.h"

#include "ocs_system/basic_delayer.h"

namespace ocs {
namespace system {

BasicDelayer::BasicDelayer(core::microseconds_t resolution) {
    gptimer_config_t config;
    memset(&config, 0, sizeof(config));

    config.clk_src = GPTIMER_CLK_SRC_DEFAULT;
    config.direction = GPTIMER_COUNT_UP;
    config.resolution_hz = resolution;

    ESP_ERROR_CHECK(gptimer_new_timer(&config, &handle_));
    configASSERT(handle_);
}

BasicDelayer::~BasicDelayer() {
    ESP_ERROR_CHECK(gptimer_disable(handle_));
    ESP_ERROR_CHECK(gptimer_del_timer(handle_));
}

} // namespace system
} // namespace ocs
