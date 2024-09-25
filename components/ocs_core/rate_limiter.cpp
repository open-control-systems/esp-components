/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/rate_limiter.h"
#include "ocs_core/time_ops.h"

namespace ocs {
namespace core {

RateLimiter::RateLimiter(core::IClock& clock, core::microseconds_t interval)
    : interval_(interval)
    , clock_(clock) {
    configASSERT(interval > 0);
}

bool RateLimiter::allow() {
    const auto now = clock_.now();

    if (start_) {
        if (!TimeOps::after(start_, now, interval_)) {
            return false;
        }
    }

    start_ = now;
    return true;
}

} // namespace core
} // namespace ocs
