/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace scheduler {

class AsyncTaskScheduler : public core::NonCopyable<> {
public:
    using TaskPtr = std::unique_ptr<ITask>;

    //! Maximum number of tasks to which a scheduler can deliver asynchronous events.
    //!
    //! @remarks
    //!  8 high bits are used by the FreeRTOS itself.
    static const unsigned max_task_count = (sizeof(ITask::Event) * 8) - 8;

    //! Register a new task to which asynchronous events should be scheduled.
    //!
    //! @return
    //!  A valid pointer if the task was registered properly,
    //!  nullptr if task was already registered,
    //!  nullptr if maximum number of tasks were already registered.
    TaskPtr add(ITask& task);

    //! Handle asynchronous events.
    //!
    //! @remarks
    //!  Blocking call.
    void run();

    //! Wait for the asynchronous events.
    void wait(TickType_t wait = portMAX_DELAY);

private:
    core::StaticEventGroup event_group_;

    ITask::Event bits_all_ { 0 };

    std::vector<std::pair<ITask::Event, ITask*>> tasks_;
};

} // namespace scheduler
} // namespace ocs
