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
#include "ocs_core/spmc_node.h"
#include "ocs_io/adc/iadc.h"
#include "ocs_sensor/soil/soil_status.h"

namespace ocs {
namespace sensor {
namespace soil {

class AnalogSensor : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Various sensor characteristics.
    struct Data {
        int raw { 0 };
        int voltage { 0 };
        int moisture { 0 };
        SoilStatus prev_status { SoilStatus::None };
        SoilStatus curr_status { SoilStatus::None };
        core::Time prev_status_duration { 0 };
        core::Time curr_status_duration { 0 };
        uint64_t write_count { 0 };
        uint16_t status_len { 0 };
        int16_t status_pos { 0 };
    };

    struct Params {
        unsigned value_min { 0 };
        unsigned value_max { 0 };
    };

    //! Initialize.
    AnalogSensor(io::adc::IAdc& adc, control::FsmBlock& fsm_block, Params params);

    //! Read sensor data.
    status::StatusCode run() override;

    //! Return the latest sensor data.
    Data get_data() const;

private:
    int calculate_moisture_(int raw) const;
    SoilStatus calculate_status_(int raw) const;
    int16_t calculate_status_position_(int raw) const;

    void update_data_(int raw, int voltage);

    // Saturated, Wet, Depletion, Dry.
    static const uint8_t status_count_ { 4 };

    const Params params_;

    io::adc::IAdc& adc_;
    control::FsmBlock& fsm_block_;

    uint16_t status_len_ { 0 };

    core::SpmcNode<Data> data_;
};

} // namespace soil
} // namespace sensor
} // namespace ocs
