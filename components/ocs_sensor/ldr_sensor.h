/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/adc_store.h"
#include "ocs_io/iadc.h"
#include "ocs_sensor/basic_sensor.h"

namespace ocs {
namespace sensor {

//! Various sensor characteristics.
struct LdrSensorData {
    int raw { 0 };
    int voltage { 0 };
    int lightness { 0 };
};

class LdrSensor : public BasicSensor<LdrSensorData>, public core::NonCopyable<> {
public:
    struct Params {
        unsigned value_min { 0 };
        unsigned value_max { 0 };
        adc_channel_t adc_channel { ADC_CHANNEL_0 };
    };

    //! Initialize.
    LdrSensor(io::AdcStore& adc_store, Params params);

    //! Read sensor data.
    status::StatusCode run() override;

private:
    int calculate_lightness_(int raw) const;

    void update_data_(int raw, int voltage);

    const Params params_;

    io::IAdc* adc_ { nullptr };
};

} // namespace sensor
} // namespace ocs
