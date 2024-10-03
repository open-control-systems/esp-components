/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_core/lock_guard.h"
#include "ocs_core/static_mutex.h"

namespace ocs {
namespace core {

TEST_CASE("Static mutex: lock/unlock", "[ocs_core], [static_mutex]") {
    StaticMutex mu;
    LockGuard lock(mu);
}

} // namespace core
} // namespace ocs
