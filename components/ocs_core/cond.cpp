/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_core/cond.h"

namespace ocs {
namespace core {

Cond::Cond(StaticMutex& mu)
    : mu_(mu) {
}

bool Cond::wait(TickType_t wait) {
    auto task_handle = xTaskGetCurrentTaskHandle();
    tasks_.push_back(task_handle);

    mu_.unlock();
    bool ret = ulTaskNotifyTake(pdTRUE, wait) != 0;
    mu_.lock();

    return ret;
}

void Cond::signal() {
    if (tasks_.empty()) {
        return;
    }

    auto task = tasks_.front();
    tasks_.pop_front();

    xTaskNotifyGive(task);
}

void Cond::broadcast() {
    while (!tasks_.empty()) {
        auto task = tasks_.front();
        tasks_.pop_front();

        xTaskNotifyGive(task);
    }
}

} // namespace core
} // namespace ocs
