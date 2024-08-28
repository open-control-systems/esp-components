/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "esp_log.h"

#include "freertos/FreeRTOSConfig.h"

#include "ocs_io/basic_gpio.h"

namespace ocs {
namespace io {

namespace {

const char* log_tag = "basic-gpio";

} // namespace

BasicGpio::BasicGpio(const char* id, gpio_num_t gpio, bool enable_value)
    : id_(id)
    , gpio_(gpio)
    , enable_value_(enable_value) {
}

int BasicGpio::get() {
    return gpio_get_level(gpio_);
}

status::StatusCode BasicGpio::flip() {
    const auto err = gpio_set_level(gpio_, !gpio_get_level(gpio_));
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "flip failed: id=%s err=%s", id_.c_str(), esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode BasicGpio::turn_on() {
    const auto err = gpio_set_level(gpio_, enable_value_);
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "turn on failed: id=%s err=%s", id_.c_str(),
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode BasicGpio::turn_off() {
    const auto err = gpio_set_level(gpio_, !enable_value_);
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "turn off failed: id=%s err=%s", id_.c_str(),
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode BasicGpio::set_direction(IGpio::Direction direction) {
    gpio_mode_t mode = GPIO_MODE_DISABLE;

    switch (direction) {
    case IGpio::Direction::Output:
        mode = GPIO_MODE_OUTPUT;
        break;

    case IGpio::Direction::Input:
        mode = GPIO_MODE_INPUT;
        break;

    default:
        break;
    }

    configASSERT(mode != GPIO_MODE_DISABLE);

    const auto err = gpio_set_direction(gpio_, mode);
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "gpio_set_direction(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace io
} // namespace ocs
