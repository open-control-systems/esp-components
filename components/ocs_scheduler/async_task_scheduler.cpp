/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/bit_ops.h"
#include "ocs_core/log.h"
#include "ocs_scheduler/async_task.h"
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace scheduler {

namespace {

const char* log_tag = "async-task-scheduler";

} // namespace

AsyncTaskScheduler::AsyncTaskScheduler() {
    // Ensure memory won't be reallocated, otherwise pointers, returned by
    // add() function, will point to the invalid memory region.
    nodes_.reserve(max_task_count);
}

unsigned AsyncTaskScheduler::count() const {
    return nodes_.size();
}

ITask* AsyncTaskScheduler::add(ITask& task, const char* id) {
    if (nodes_.size() == max_task_count) {
        return nullptr;
    }

    for (const auto& node : nodes_) {
        if (node.task == &task) {
            ocs_loge(log_tag, "task already exists: event=%lu", node.event);
            return nullptr;
        }
    }

    const ITask::Event event = core::BitOps::mask(nodes_.size());

    TaskNode node(&task, event_group_.get(), event, id);
    nodes_.emplace_back(node);

    bits_all_ |= event;

    return &nodes_.back().async_task;
}

status::StatusCode AsyncTaskScheduler::wait(TickType_t wait) {
    configASSERT(nodes_.size());

    const EventBits_t bits =
        xEventGroupWaitBits(event_group_.get(), bits_all_, pdTRUE, pdFALSE, wait);

    return handle_events_(bits);
}

status::StatusCode AsyncTaskScheduler::handle_events_(EventBits_t bits) {
    for (auto& node : nodes_) {
        if (bits & node.event) {
            const auto code = node.task->run();
            if (code != status::StatusCode::OK) {
                ocs_loge(log_tag, "failed to run task: id=%s code=%s", node.id.c_str(),
                         status::code_to_str(code));
            }
        }
    }

    return status::StatusCode::OK;
}

} // namespace scheduler
} // namespace ocs
