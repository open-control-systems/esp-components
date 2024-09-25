/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/lock_guard.h"
#include "ocs_core/log.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace scheduler {

namespace {

const char* log_tag = "async-func-scheduler";

} // namespace

AsyncFuncScheduler::AsyncFuncScheduler(unsigned max_event_count)
    : max_event_count_(max_event_count) {
    configASSERT(max_event_count_);

    read_queue_.reset(new (std::nothrow) Queue());
    configASSERT(read_queue_);

    write_queue_.reset(new (std::nothrow) Queue());
    configASSERT(write_queue_);
}

status::StatusCode AsyncFuncScheduler::run() {
    {
        core::LockGuard lock(mu_);

        if (!write_queue_->size()) {
            return status::StatusCode::OK;
        }

        std::swap(write_queue_, read_queue_);
    }

    for (auto fn : *read_queue_) {
        const auto code = fn();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag, "failed to handle async event: %s",
                     status::code_to_str(code));
        }
    }

    read_queue_->clear();

    return status::StatusCode::OK;
}

AsyncFuncScheduler::FuturePtr AsyncFuncScheduler::add(AsyncFuncScheduler::Func func) {
    core::LockGuard lock(mu_);

    if (write_queue_->size() == max_event_count_) {
        return nullptr;
    }

    FuturePtr future(new (std::nothrow) core::Future());
    if (!future) {
        return nullptr;
    }

    write_queue_->push_back([func, future]() {
        return future->notify(func());
    });

    return future;
}

} // namespace scheduler
} // namespace ocs
