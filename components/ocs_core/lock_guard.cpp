/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_core/lock_guard.h"

namespace ocs {
namespace core {

LockGuard::LockGuard(ILocker& locker)
    : locker_(locker) {
    locker_.lock();
}

LockGuard::~LockGuard() {
    locker_.unlock();
}

} // namespace core
} // namespace ocs
