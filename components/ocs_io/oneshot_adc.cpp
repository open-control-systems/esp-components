/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_io/oneshot_adc.h"
#include "ocs_core/log.h"

namespace ocs {
namespace io {

namespace {

const char* log_tag = "oneshot-adc";

} // namespace

OneshotAdc::OneshotAdc(adc_channel_t channel,
                       adc_oneshot_unit_handle_t unit_handle,
                       adc_cali_handle_t calibration_handle)
    : channel_(channel)
    , unit_handle_(unit_handle)
    , calibration_handle_(calibration_handle) {
}

IAdc::Result OneshotAdc::read() {
    int raw = 0;

    const auto err = adc_oneshot_read(unit_handle_, channel_, &raw);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "adc_oneshot_read(): channel=%u err=%s", channel_,
                 esp_err_to_name(err));

        return { status::StatusCode::Error, -1 };
    }

    return { status::StatusCode::OK, raw };
}

IAdc::Result OneshotAdc::convert(int raw) {
    int voltage = 0;

    const auto err = adc_cali_raw_to_voltage(calibration_handle_, raw, &voltage);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "adc_cali_raw_to_voltage(): channel=%u err=%s", channel_,
                 esp_err_to_name(err));

        return { status::StatusCode::Error, -1 };
    }

    return { status::StatusCode::OK, voltage };
}

} // namespace io
} // namespace ocs
