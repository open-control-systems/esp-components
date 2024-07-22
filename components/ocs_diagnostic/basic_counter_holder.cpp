/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_diagnostic/basic_counter_holder.h"

namespace ocs {
namespace diagnostic {

const BasicCounterHolder::CounterList& BasicCounterHolder::get_counters() const {
    return counters_;
}

void BasicCounterHolder::add(ICounter& counter) {
    counters_.emplace_back(&counter);
}

} // namespace diagnostic
} // namespace ocs
