/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/time.h"
#include "ocs_scheduler/high_resolution_timer.h"
#include "ocs_sensor/yl69_sensor.h"

namespace ocs {
namespace sensor {

const char* YL69SensorData::soil_status_to_str(YL69SensorData::SoilStatus status) {
    switch (status) {
    case YL69SensorData::SoilStatus::Dry:
        return "dry";

    case YL69SensorData::SoilStatus::Wet:
        return "wet";

    default:
        break;
    }

    return "<none>";
}

YL69Sensor::YL69Sensor(core::IClock& clock,
                       io::AdcStore& adc_store,
                       storage::IStorage& storage,
                       system::FanoutRebootHandler& reboot_handler,
                       scheduler::AsyncTaskScheduler& task_scheduler,
                       scheduler::TimerStore& timer_store,
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
        static_cast<diagnostic::StateCounter::State>(YL69SensorData::SoilStatus::Dry)));
    configASSERT(dry_state_task_);

    counter_holder.add(*dry_state_task_);
    reboot_handler.add(*dry_state_task_);

    wet_state_task_.reset(new (std::nothrow) diagnostic::StateCounter(
        storage, clock, "c_sen_yl69_wet", core::Second,
        static_cast<diagnostic::StateCounter::State>(YL69SensorData::SoilStatus::Wet)));
    configASSERT(wet_state_task_);

    counter_holder.add(*wet_state_task_);
    reboot_handler.add(*wet_state_task_);

    fanout_task_.reset(new (std::nothrow) scheduler::FanoutTask());
    configASSERT(fanout_task_);

    fanout_task_->add(*dry_state_task_);
    fanout_task_->add(*wet_state_task_);

    fanout_task_async_ = task_scheduler.add(*fanout_task_);
    configASSERT(fanout_task_async_);

    task_timer_.reset(new (std::nothrow) scheduler::HighResolutionTimer(
        *fanout_task_async_, task_id, core::Minute * 30));
    configASSERT(task_timer_);

    timer_store.add(*task_timer_);
}

status::StatusCode YL69Sensor::run() {
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

int YL69Sensor::calculate_moisture_(int raw) const {
    if (raw >= params_.value_max) {
        return 0;
    }

    if (raw <= params_.value_min) {
        return 100;
    }

    const int range = params_.value_max - params_.value_min;
    const int offset = raw - params_.value_min;
    const float loss = static_cast<float>(offset) / range;
    const float remain = 1 - loss;

    return 100 * remain;
}

YL69SensorData::SoilStatus YL69Sensor::calculate_status_(int raw) const {
    if (raw >= params_.value_max) {
        return YL69SensorData::SoilStatus::Dry;
    }

    return YL69SensorData::SoilStatus::Wet;
}

YL69SensorData::SoilStatus YL69Sensor::update_data_(int raw, int voltage) {
    YL69SensorData data;

    data.raw = raw;
    data.voltage = voltage;
    data.moisture = calculate_moisture_(raw);
    data.status = calculate_status_(raw);

    set_data_(data);

    return data.status;
}

} // namespace sensor
} // namespace ocs
