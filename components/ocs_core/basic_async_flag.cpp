/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_core/basic_async_flag.h"

namespace ocs {
namespace core {

status::StatusCode BasicAsyncFlag::wait(TickType_t wait) {
    task_to_notify_.store(xTaskGetCurrentTaskHandle(), std::memory_order_release);

    const auto count = ulTaskNotifyTake(pdTRUE, wait);
    return count != 0 ? status::StatusCode::OK : status::StatusCode::Error;
}

TaskHandle_t BasicAsyncFlag::acquire_task_() {
    return task_to_notify_.exchange(nullptr, std::memory_order_acq_rel);
}

} // namespace core
} // namespace ocs
