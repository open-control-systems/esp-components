/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <utility>
#include <vector>

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/oneshot_adc.h"

namespace ocs {
namespace io {

class AdcStore : public core::NonCopyable<> {
public:
    struct Params {
        adc_unit_t unit { ADC_UNIT_1 };
        adc_atten_t atten { ADC_ATTEN_DB_0 };
        adc_bitwidth_t bitwidth { ADC_BITWIDTH_DEFAULT };
    };

    //! Configure ADC unit.
    explicit AdcStore(Params);

    //! Release ADC unit resources.
    ~AdcStore();

    //! Configure ADC reading for @p channel.
    //!
    //! @return
    //!  A valid pointer if the ADC was configured properly.
    //!  nullptr if ADC was already configured.
    //!  nullptr if maximum number of channels were already configured.
    IAdc* add(adc_channel_t channel);

private:
    const Params params_;

    const unsigned max_channel_count_ { 0 };

    adc_oneshot_chan_cfg_t config_;
    adc_oneshot_unit_init_cfg_t unit_config_;
    adc_cali_line_fitting_config_t calibration_config_;

    adc_oneshot_unit_handle_t unit_handle_ { nullptr };
    adc_cali_handle_t calibration_handle_ { nullptr };

    std::vector<std::pair<adc_channel_t, OneshotAdc>> adcs_;
};

} // namespace io
} // namespace ocs
