/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace ocs {
namespace scheduler {

//! Estimate required delay of FreeRTOS task.
class IDelayEstimator {
public:
    //! Destroy.
    virtual ~IDelayEstimator() = default;

    //! Begin delay estimation.
    virtual void begin() = 0;

    //! Estimate required delay.
    virtual TickType_t estimate() = 0;
};

} // namespace scheduler
} // namespace ocs
