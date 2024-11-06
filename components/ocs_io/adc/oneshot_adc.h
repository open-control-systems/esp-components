/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_oneshot.h"

#include "ocs_io/adc/iadc.h"
#include "ocs_io/adc/types.h"

namespace ocs {
namespace io {
namespace adc {

class OneshotAdc : public IAdc {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p channel - ADC channel to read value from.
    //!  - @p unit_handle - handle to operate with ADC unit.
    //!  - @p calibration_handle - handle to convert raw ADC value into voltage.
    OneshotAdc(Channel channel,
               adc_oneshot_unit_handle_t unit_handle,
               adc_cali_handle_t calibration_handle);

    //! Read raw ADC value from the configured channel.
    IAdc::Result read() override;

    //! Convert raw ADC value into voltage, in mV.
    IAdc::Result convert(int raw) override;

private:
    Channel channel_ { ADC_CHANNEL_0 };
    adc_oneshot_unit_handle_t unit_handle_ { nullptr };
    adc_cali_handle_t calibration_handle_ { nullptr };
};

} // namespace adc
} // namespace io
} // namespace ocs
