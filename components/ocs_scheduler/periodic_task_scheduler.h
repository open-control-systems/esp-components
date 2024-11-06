/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ocs_core/iclock.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/rate_limiter.h"
#include "ocs_scheduler/idelay_estimator.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itask_scheduler.h"

namespace ocs {
namespace scheduler {

class PeriodicTaskScheduler : public ITaskScheduler, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p clock to check if it's a time to run a task.
    //!  - @p estimator to estimate the required delay after each round of execution.
    //!  - @p id to distinguish one scheduler from another.
    //!  - @p max_count - maximum number of tasks the scheduler can handle.
    PeriodicTaskScheduler(core::IClock& clock,
                          IDelayEstimator& estimator,
                          const char* id,
                          unsigned max_count);

    //! Return the maximum configured number of tasks a scheduler can handle.
    unsigned max_count() const override;

    //! Return the number of registered tasks.
    unsigned count() const override;

    //! Add task to be executed periodically once per interval.
    //!
    //! @params
    //!  - @p task - task to be executed periodically.
    //!  - @p id - unique task identifier.
    //!  - @p interval - task running frequency.
    //!
    //! @notes
    //! It's possible for the task's frequency to drift over time. There are several
    //! reasons for this:
    //!  - This scheduler can't be considered as a precise scheduler, since all tasks
    //!    are running on the same FreeRTOS task. This task can be preempted by other
    //!    tasks with higher priority.
    //!  - If there are too many tasks added to the same scheduler, it is possible that
    //!    the total time required to run all these tasks will be greater then the
    //!    minimum periodic interval.
    status::StatusCode add(ITask& task, const char* id, core::Time interval);

    //! Start tasks scheduling.
    status::StatusCode start() override;

    //! Stop tasks scheduling.
    status::StatusCode stop() override;

    //! Run all periodic tasks.
    status::StatusCode run() override;

private:
    class Node : public ITask, public core::NonCopyable<> {
    public:
        Node(core::IClock& clock, ITask& task, const char* id, core::Time interval);

        status::StatusCode run() override;

        const char* id() const;

    private:
        const std::string id_;

        ITask& task_;

        core::RateLimiter limiter_;
    };

    using NodePtr = std::shared_ptr<Node>;
    using NodeList = std::vector<NodePtr>;

    void run_();

    const unsigned max_count_ { 0 };
    const std::string log_tag_;

    core::Time task_min_interval_ { INT64_MAX };

    core::Time total_ts_min_ { INT64_MAX };
    core::Time total_ts_max_ { INT64_MIN };

    core::IClock& clock_;
    IDelayEstimator& estimator_;

    NodeList nodes_;
};

} // namespace scheduler
} // namespace ocs
