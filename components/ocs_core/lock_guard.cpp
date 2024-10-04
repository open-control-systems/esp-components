/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_core/lock_guard.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace core {

namespace {

const char* log_tag = "lock_guard";

} // namespace

LockGuard::LockGuard(ILocker& locker, TickType_t wait)
    : locker_(locker) {
    const auto code = locker_.lock(wait);
    if (unlikely(code != status::StatusCode::OK)) {
        ocs_loge(log_tag, "failed to lock the resource: %s", status::code_to_str(code));
    }
}

LockGuard::~LockGuard() {
    const auto code = locker_.unlock();
    if (unlikely(code != status::StatusCode::OK)) {
        ocs_loge(log_tag, "failed to unlock the resource: %s", status::code_to_str(code));
    }
}

} // namespace core
} // namespace ocs
