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

const char* YL69Sensor::soil_status_to_str(YL69Sensor::SoilStatus status) {
    switch (status) {
    case YL69Sensor::SoilStatus::Dry:
        return "dry";

    case YL69Sensor::SoilStatus::Wet:
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
                       diagnostic::BasicCounterHolder& counter_holder)
    : threshold_dry_(CONFIG_OCS_SENSOR_YL69_DRY_THRESHOLD)
    , threshold_wet_(CONFIG_OCS_SENSOR_YL69_WET_THRESHOLD) {
    configASSERT(threshold_dry_ > threshold_wet_);

    adc_ = adc_store.add(static_cast<adc_channel_t>(CONFIG_OCS_SENSOR_YL69_ADC_CHANNEL));
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

    fanout_task_async_ = task_scheduler.add(*fanout_task_);
    configASSERT(fanout_task_async_);

    task_timer_.reset(new (std::nothrow) scheduler::HighResolutionTimer(
        *fanout_task_async_, "YL69-sensor-save-status", core::Minute * 30));
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

    const auto status =
        read_result.value > threshold_dry_ ? SoilStatus::Dry : SoilStatus::Wet;

    dry_state_task_->update(static_cast<diagnostic::StateCounter::State>(status));
    wet_state_task_->update(static_cast<diagnostic::StateCounter::State>(status));

    update_data_(read_result.value, conv_result.value, status);

    return status::StatusCode::OK;
}

YL69Sensor::Data YL69Sensor::get_data() const {
    return data_;
}

int YL69Sensor::calculate_moisture_(int raw) const {
    if (raw >= threshold_dry_) {
        return 0;
    }

    if (raw <= threshold_wet_) {
        return 100;
    }

    const int max_value = threshold_dry_ - threshold_wet_;
    const float current_value = static_cast<float>(threshold_dry_ - raw);

    return 100 * (current_value / max_value);
}

void YL69Sensor::update_data_(int raw, int voltage, SoilStatus status) {
    Data data;
    data.raw = raw;
    data.voltage = voltage;
    data.moisture = calculate_moisture_(raw);
    data.status = status;

    data_ = data;
}

} // namespace sensor
} // namespace ocs
