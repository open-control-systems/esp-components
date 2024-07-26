/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_diagnostic/time_counter.h"

namespace ocs {
namespace diagnostic {

TimeCounter::TimeCounter(core::IClock& clock,
                         const char* id,
                         core::microseconds_t resolution)
    : BasicCounter(id)
    , resolution_(resolution)
    , clock_(clock) {
}

ICounter::Value TimeCounter::get() const {
    return clock_.now() / resolution_;
}

} // namespace diagnostic
} // namespace ocs
