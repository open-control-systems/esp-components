/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/time.h"
#include "ocs_sensor/yl69/sensor.h"

namespace ocs {
namespace sensor {
namespace yl69 {

Sensor::Sensor(core::IClock& clock,
               io::AdcStore& adc_store,
               storage::IStorage& storage,
               system::FanoutRebootHandler& reboot_handler,
               scheduler::ITaskScheduler& task_scheduler,
               diagnostic::BasicCounterHolder& counter_holder,
               const char* sensor_id,
               const char* task_id,
               Params params)
    : BasicSensor(sensor_id)
    , params_(params) {
    configASSERT(params_.value_min < params_.value_max);

    adc_ = adc_store.add(params_.adc_channel);
    configASSERT(adc_);

    dry_state_task_.reset(new (std::nothrow) diagnostic::StateCounter(
        storage, clock, "c_sen_yl69_dry", core::Second,
        static_cast<diagnostic::StateCounter::State>(SoilStatus::Dry)));
    configASSERT(dry_state_task_);

    counter_holder.add(*dry_state_task_);
    reboot_handler.add(*dry_state_task_);

    wet_state_task_.reset(new (std::nothrow) diagnostic::StateCounter(
        storage, clock, "c_sen_yl69_wet", core::Second,
        static_cast<diagnostic::StateCounter::State>(SoilStatus::Wet)));
    configASSERT(wet_state_task_);

    counter_holder.add(*wet_state_task_);
    reboot_handler.add(*wet_state_task_);

    fanout_task_.reset(new (std::nothrow) scheduler::FanoutTask());
    configASSERT(fanout_task_);

    fanout_task_->add(*dry_state_task_);
    fanout_task_->add(*wet_state_task_);

    configASSERT(task_scheduler.add(*fanout_task_, task_id, core::Minute * 30)
                 == status::StatusCode::OK);
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

    const auto status = update_data_(read_result.value, conv_result.value);

    dry_state_task_->update(static_cast<diagnostic::StateCounter::State>(status));
    wet_state_task_->update(static_cast<diagnostic::StateCounter::State>(status));

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

    const unsigned interval_len =
        (params_.value_max - params_.value_min) / interval_count_;

    if (raw < params_.value_min + interval_len) {
        return SoilStatus::Saturated;
    }
    if (raw < params_.value_min + interval_len + interval_len) {
        return SoilStatus::Wet;
    }
    if (raw < params_.value_min + interval_len + interval_len + interval_len) {
        return SoilStatus::Depletion;
    }

    return SoilStatus::Dry;
}

SoilStatus Sensor::update_data_(int raw, int voltage) {
    SensorData data;

    data.raw = raw;
    data.voltage = voltage;
    data.moisture = calculate_moisture_(raw);
    data.status = calculate_status_(raw);

    set_data_(data);

    return data.status;
}

} // namespace yl69
} // namespace sensor
} // namespace ocs
