/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace io {
namespace adc {

class IAdc {
public:
    //! ADC operation result.
    struct Result {
        status::StatusCode code { status::StatusCode::OK };
        int value { 0 };
    };

    //! Destroy.
    virtual ~IAdc() = default;

    //! Read raw ADC value.
    virtual Result read() = 0;

    //! Convert raw ADC value into voltage, in mV.
    virtual Result convert(int raw) = 0;
};

} // namespace adc
} // namespace io
} // namespace ocs
