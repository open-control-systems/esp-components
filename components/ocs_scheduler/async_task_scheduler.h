/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_scheduler/idelay_estimator.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_scheduler/itimer.h"

namespace ocs {
namespace scheduler {

class AsyncTaskScheduler : public ITaskScheduler, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p estimator to estimate the required delay after each round of execution.
    //!  - @p id to distinguish one scheduler from another.
    AsyncTaskScheduler(IDelayEstimator& estimator, const char* id);

    //! Maximum number of tasks to which a scheduler can deliver asynchronous events.
    unsigned max_count() const override;

    //! Return number of registered tasks to which asynchronous events are delivered.
    unsigned count() const override;

    //! Add task to be executed once per interval.
    //!
    //! @params
    //!  - @p task to which asynchronous events should be scheduled.
    //!  - @p id to distinguish one task from another.
    //!  - @p interval - task running frequency, microseconds resolution.
    //!
    //! @notes
    //!  This scheduler can be considered as a precise scheduler. Once per interval
    //!  high-resolution timer schedules the task execution, which is handled during the
    //!  run() call.
    status::StatusCode add(ITask& task, const char* id, core::Time interval) override;

    //! Start tasks scheduling.
    status::StatusCode start() override;

    //! Stop tasks scheduling.
    status::StatusCode stop() override;

    //! Wait for the asynchronous tasks.
    status::StatusCode run() override;

private:
    class Node : public ITask, public core::NonCopyable<> {
    public:
        Node(ITask& task,
             EventGroupHandle_t event_group,
             EventBits_t event,
             core::Time interval,
             const char* id);

        status::StatusCode run() override;

        const char* id() const;
        EventBits_t event() const;

        status::StatusCode start();
        status::StatusCode stop();

    private:
        const std::string id_;
        const EventBits_t event_ { 0 };

        ITask& task_;

        std::unique_ptr<ITask> async_task_;
        std::unique_ptr<ITimer> timer_;
    };

    using NodePtr = std::shared_ptr<Node>;

    void run_(EventBits_t bits);

    const std::string log_tag_;

    IDelayEstimator& estimator_;

    std::vector<NodePtr> nodes_;

    core::StaticEventGroup event_group_;
    EventBits_t bits_all_ { 0 };
};

} // namespace scheduler
} // namespace ocs
