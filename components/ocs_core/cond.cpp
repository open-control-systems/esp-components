/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_core/cond.h"
#include "ocs_core/lock_guard.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace core {

Cond::Cond(ILocker& locker)
    : locker_(locker) {
}

status::StatusCode Cond::wait(TickType_t wait) {
    append_task_();

    OCS_STATUS_RETURN_ON_ERROR(locker_.unlock());
    const auto count = ulTaskNotifyTake(pdTRUE, wait);
    OCS_STATUS_RETURN_ON_ERROR(locker_.lock());

    return count != 0 ? status::StatusCode::OK : status::StatusCode::Error;
}

status::StatusCode Cond::signal() {
    core::LockGuard lock(mu_);

    if (!tasks_.empty()) {
        return signal_();
    }

    return status::StatusCode::OK;
}

status::StatusCode Cond::broadcast() {
    core::LockGuard lock(mu_);

    while (!tasks_.empty()) {
        OCS_STATUS_RETURN_ON_ERROR(signal_());
    }

    return status::StatusCode::OK;
}

void Cond::append_task_() {
    core::LockGuard lock(mu_);

    tasks_.push_back(xTaskGetCurrentTaskHandle());
}

status::StatusCode Cond::signal_() {
    auto task = tasks_.front();
    tasks_.pop_front();

    OCS_STATUS_RETURN_ON_FALSE(xTaskNotifyGive(task) == pdTRUE,
                               status::StatusCode::Error);

    return status::StatusCode::OK;
}

} // namespace core
} // namespace ocs
