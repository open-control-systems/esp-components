/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_core/cond.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace core {

Cond::Cond(ILocker& locker)
    : locker_(locker) {
}

status::StatusCode Cond::wait(TickType_t wait) {
    auto task_handle = xTaskGetCurrentTaskHandle();
    tasks_.push_back(task_handle);

    locker_.unlock();
    const auto count = ulTaskNotifyTake(pdTRUE, wait);
    locker_.lock();

    return count != 0 ? status::StatusCode::OK : status::StatusCode::Error;
}

status::StatusCode Cond::signal() {
    if (!tasks_.empty()) {
        return signal_();
    }

    return status::StatusCode::OK;
}

status::StatusCode Cond::broadcast() {
    while (!tasks_.empty()) {
        OCS_STATUS_RETURN_ON_ERROR(signal_());
    }

    return status::StatusCode::OK;
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
