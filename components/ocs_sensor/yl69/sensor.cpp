/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_core/time.h"
#include "ocs_sensor/yl69/sensor.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace sensor {
namespace yl69 {

namespace {

SoilStatus parse_status(control::FsmBlock::State state) {
    if (state < static_cast<control::FsmBlock::State>(SoilStatus::None)
        && state >= static_cast<control::FsmBlock::State>(SoilStatus::Last)) {
        return SoilStatus::None;
    }

    return static_cast<SoilStatus>(state);
}

const char* log_tag = "sensor_yl69";

} // namespace

Sensor::Sensor(io::AdcStore& adc_store,
               control::FsmBlock& fsm_block,
               const char* sensor_id,
               Params params)
    : BasicSensor(sensor_id)
    , params_(params)
    , fsm_block_(fsm_block) {
    configASSERT(params_.value_min < params_.value_max);

    adc_ = adc_store.add(params_.adc_channel);
    configASSERT(adc_);

    status_len_ = (params_.value_max - params_.value_min) / status_count_;
}

status::StatusCode Sensor::run() {
    const auto read_result = adc_->read();
    if (read_result.code != status::StatusCode::OK) {
        return read_result.code;
    }

    const auto conv_result = adc_->convert(read_result.value);
    if (conv_result.code != status::StatusCode::OK) {
        return conv_result.code;
    }

    fsm_block_.update();

    fsm_block_.set_next(
        static_cast<control::FsmBlock::State>(calculate_status_(read_result.value)));

    if (fsm_block_.is_in_transit()) {
        const auto code = fsm_block_.transit();
        if (code != status::StatusCode::OK) {
            ocs_logw(
                log_tag,
                "failed to transit block to the next state: id=%s code=%s prev_state=%u "
                "curr_state=%u prev_dur=%lli curr_dur=%lli",
                id(), status::code_to_str(code), fsm_block_.previous_state(),
                fsm_block_.current_state(), fsm_block_.previous_state_duration(),
                fsm_block_.current_state_duration());
        }
    }

    update_data_(read_result.value, conv_result.value);

    return status::StatusCode::OK;
}

int Sensor::calculate_moisture_(int raw) const {
    if (raw > params_.value_max) {
        return 0;
    }

    if (raw < params_.value_min) {
        return 100;
    }

    const int range = params_.value_max - params_.value_min;
    const int offset = raw - params_.value_min;
    const float loss = static_cast<float>(offset) / range;
    const float remain = 1 - loss;

    return 100 * remain;
}

SoilStatus Sensor::calculate_status_(int raw) const {
    if (raw < params_.value_min || raw > params_.value_max) {
        return SoilStatus::Error;
    }

    if (raw < params_.value_min + status_len_) {
        return SoilStatus::Saturated;
    }
    if (raw < params_.value_min + status_len_ + status_len_) {
        return SoilStatus::Wet;
    }
    if (raw < params_.value_min + status_len_ + status_len_ + status_len_) {
        return SoilStatus::Depletion;
    }

    return SoilStatus::Dry;
}

int16_t Sensor::calculate_status_position_(int raw) const {
    if (raw < params_.value_min) {
        return -1;
    }

    const auto offset = raw - params_.value_min;
    const auto count = offset / status_len_;
    const auto pos = offset - (status_len_ * count);

    return pos;
}

void Sensor::update_data_(int raw, int voltage) {
    SensorData data;

    data.raw = raw;
    data.voltage = voltage;
    data.moisture = calculate_moisture_(raw);
    data.prev_status = parse_status(fsm_block_.previous_state());
    data.curr_status = parse_status(fsm_block_.current_state());
    data.prev_status_duration = fsm_block_.previous_state_duration();
    data.curr_status_duration = fsm_block_.current_state_duration();
    data.write_count = fsm_block_.write_count();
    data.status_len = status_len_;
    data.status_pos = calculate_status_position_(raw);

    set_data_(data);
}

} // namespace yl69
} // namespace sensor
} // namespace ocs
