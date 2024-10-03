/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <atomic>
#include <memory>

#include "ocs_control/fsm_block.h"
#include "ocs_control/ifsm_handler.h"
#include "ocs_core/noncopyable.h"
#include "ocs_io/adc_store.h"
#include "ocs_io/iadc.h"
#include "ocs_sensor/basic_sensor.h"
#include "ocs_sensor/yl69/soil_status.h"

namespace ocs {
namespace sensor {
namespace yl69 {

//! Various sensor characteristics.
struct SensorData {
    int raw { 0 };
    int voltage { 0 };
    int moisture { 0 };
    SoilStatus prev_status { SoilStatus::None };
    SoilStatus curr_status { SoilStatus::None };
    core::microseconds_t prev_status_duration { 0 };
    core::microseconds_t curr_status_duration { 0 };
    uint64_t write_count { 0 };
};

class Sensor : public scheduler::ITask,
               public BasicSensor<SensorData>,
               public core::NonCopyable<> {
public:
    struct Params {
        unsigned value_min { 0 };
        unsigned value_max { 0 };
        adc_channel_t adc_channel { ADC_CHANNEL_0 };
    };

    //! Initialize.
    Sensor(io::AdcStore& adc_store,
           control::FsmBlock& fsm_block,
           const char* sensor_id,
           Params params);

    //! Read sensor data.
    status::StatusCode run() override;

private:
    int calculate_moisture_(int raw) const;
    SoilStatus calculate_status_(int raw) const;

    void update_data_(int raw, int voltage);

    const Params params_;

    control::FsmBlock& fsm_block_;
    io::IAdc* adc_ { nullptr };
};

} // namespace yl69
} // namespace sensor
} // namespace ocs
