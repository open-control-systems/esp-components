/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "soc/soc_caps.h"

#include "ocs_core/log.h"
#include "ocs_io/adc_store.h"

namespace ocs {
namespace io {

namespace {

const char* log_tag = "adc_store";

} // namespace

AdcStore::AdcStore(AdcStore::Params params)
    : params_(params)
    , max_channel_count_(SOC_ADC_CHANNEL_NUM(params.unit)) {
    // Ensure memory won't be reallocated, otherwise pointers, returned by
    // add() function will point to the invalid memory region.
    adcs_.reserve(max_channel_count_);

    // Unit configuration.
    memset(&unit_config_, 0, sizeof(unit_config_));
    unit_config_.unit_id = params.unit;
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_config_, &unit_handle_));

    // ADC configuration.
    memset(&config_, 0, sizeof(config_));
    config_.bitwidth = params_.bitwidth;
    config_.atten = params_.atten;

    // ADC calibration configuration.
    memset(&calibration_config_, 0, sizeof(calibration_config_));
    calibration_config_.unit_id = params.unit;
    calibration_config_.atten = params_.atten;
    calibration_config_.bitwidth = params_.bitwidth;
    ESP_ERROR_CHECK(
        adc_cali_create_scheme_line_fitting(&calibration_config_, &calibration_handle_));
}

AdcStore::~AdcStore() {
    ESP_ERROR_CHECK(adc_oneshot_del_unit(unit_handle_));
    ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(calibration_handle_));
}

IAdc* AdcStore::add(adc_channel_t channel) {
    for (const auto& adc : adcs_) {
        if (adc.first == channel) {
            ocs_loge(log_tag, "channel %u already configured", channel);
            return nullptr;
        }
    }

    const auto err = adc_oneshot_config_channel(unit_handle_, channel, &config_);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "adc_oneshot_config_channel(): channel=%u err=%s", channel,
                 esp_err_to_name(err));

        return nullptr;
    }

    adcs_.emplace_back(std::pair<adc_channel_t, OneshotAdc>(
        channel, OneshotAdc(channel, unit_handle_, calibration_handle_)));

    return &adcs_.back().second;
}

} // namespace io
} // namespace ocs
