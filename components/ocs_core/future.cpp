/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_core/future.h"
#include "ocs_core/lock_guard.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace core {

Future::Future()
    : cond_(mu_) {
}

status::StatusCode Future::notify(status::StatusCode code) {
    core::LockGuard lock(mu_);

    code_ = code;
    return cond_.broadcast();
}

status::StatusCode Future::wait(TickType_t wait) {
    core::LockGuard lock(mu_);

    while (code_ == status::StatusCode::Last) {
        OCS_STATUS_RETURN_ON_ERROR(cond_.wait(wait));
    }

    return status::StatusCode::OK;
}

status::StatusCode Future::code() const {
    core::LockGuard lock(mu_);

    return code_;
}

} // namespace core
} // namespace ocs
