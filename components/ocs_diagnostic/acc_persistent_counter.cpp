/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_diagnostic/acc_persistent_counter.h"

namespace ocs {
namespace diagnostic {

AccPersistentCounter::AccPersistentCounter(storage::IStorage& storage, ICounter& counter)
    : BasicPersistentCounter(storage, counter) {
}

ICounter::Value AccPersistentCounter::get() const {
    return value_ + counter_.get();
}

} // namespace diagnostic
} // namespace ocs
