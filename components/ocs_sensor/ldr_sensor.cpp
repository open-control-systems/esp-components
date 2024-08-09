/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/ldr_sensor.h"

namespace ocs {
namespace sensor {

LdrSensor::LdrSensor(io::AdcStore& adc_store)
    : value_min_(CONFIG_OCS_SENSOR_LDR_VALUE_MIN)
    , value_max_(CONFIG_OCS_SENSOR_LDR_VALUE_MAX) {
    configASSERT(value_min_ < value_max_);

    adc_ = adc_store.add(static_cast<adc_channel_t>(CONFIG_OCS_SENSOR_LDR_ADC_CHANNEL));
    configASSERT(adc_);
}

status::StatusCode LdrSensor::run() {
    const auto read_result = adc_->read();
    if (read_result.code != status::StatusCode::OK) {
        return read_result.code;
    }

    const auto conv_result = adc_->convert(read_result.value);
    if (conv_result.code != status::StatusCode::OK) {
        return conv_result.code;
    }

    update_data_(read_result.value, conv_result.value);

    return status::StatusCode::OK;
}

LdrSensor::Data LdrSensor::get_data() const {
    return data_;
}

int LdrSensor::calculate_lightness_(int raw) const {
    if (raw >= value_max_) {
        return 100;
    }

    if (raw <= value_min_) {
        return 0;
    }

    const int range = value_max_ - value_min_;
    const int offset = raw - value_min_;
    const float lightness = static_cast<float>(offset) / range;

    return 100 * lightness;
}

void LdrSensor::update_data_(int raw, int voltage) {
    Data data;

    data.raw = raw;
    data.voltage = voltage;
    data.lightness = calculate_lightness_(raw);

    data_ = data;
}

} // namespace sensor
} // namespace ocs
