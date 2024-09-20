/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_scheduler/async_task.h"

namespace ocs {
namespace scheduler {

class AsyncTaskScheduler : public core::NonCopyable<> {
public:
    //! Initialize.
    AsyncTaskScheduler();

    //! Maximum number of tasks to which a scheduler can deliver asynchronous events.
    //!
    //! @remarks
    //!  8 high bits are used by the FreeRTOS itself.
    static const unsigned max_task_count = (sizeof(ITask::Event) * 8) - 8;

    //! Return number of registered tasks to which asynchronous events are delivered.
    unsigned count() const;

    //! Register a new task.
    //!
    //! @params
    //!  - @p task to which asynchronous events should be scheduled.
    //!  - @p id to distinguish one task from another.
    //!
    //! @return
    //!  A valid pointer if the task was registered properly,
    //!  nullptr if task was already registered,
    //!  nullptr if maximum number of tasks were already registered.
    ITask* add(ITask& task, const char* id);

    //! Wait for the asynchronous events.
    status::StatusCode wait(TickType_t wait = portMAX_DELAY);

private:
    struct TaskNode {
        TaskNode(ITask* task,
                 EventGroupHandle_t event_group,
                 ITask::Event event,
                 const char* id)
            : id(id)
            , task(task)
            , event(event)
            , async_task(event_group, event) {
        }

        const std::string id;

        ITask* task { nullptr };
        ITask::Event event { 0 };
        AsyncTask async_task;
    };

    status::StatusCode handle_events_(EventBits_t bits);

    core::StaticEventGroup event_group_;

    ITask::Event bits_all_ { 0 };

    std::vector<TaskNode> nodes_;
};

} // namespace scheduler
} // namespace ocs
