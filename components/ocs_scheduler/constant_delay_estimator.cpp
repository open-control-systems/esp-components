/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_scheduler/constant_delay_estimator.h"

namespace ocs {
namespace scheduler {

ConstantDelayEstimator::ConstantDelayEstimator(TickType_t delay)
    : delay_(delay) {
}

void ConstantDelayEstimator::begin() {
}

TickType_t ConstantDelayEstimator::estimate() {
    return delay_;
}

} // namespace scheduler
} // namespace ocs
