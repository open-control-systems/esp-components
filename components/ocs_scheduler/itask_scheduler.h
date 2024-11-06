/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/time.h"
#include "ocs_scheduler/itask.h"
#include "ocs_status/code.h"

namespace ocs {
namespace scheduler {

class ITaskScheduler {
public:
    //! Destroy.
    virtual ~ITaskScheduler() = default;

    //! Return the maximum number of tasks a scheduler can handle.
    virtual unsigned max_count() const = 0;

    //! Return the number of registered tasks.
    virtual unsigned count() const = 0;

    //! Add task to be executed periodically once per interval.
    //!
    //! @params
    //!  - @p task - task to be executed periodically.
    //!  - @p id - unique task identifier.
    //!  - @p interval - task running frequency.
    virtual status::StatusCode add(ITask& task, const char* id, core::Time interval) = 0;

    //! Start scheduling registered tasks.
    virtual status::StatusCode start() = 0;

    //! Stop scheduling registered tasks.
    virtual status::StatusCode stop() = 0;

    //! Run all registered tasks.
    virtual status::StatusCode run() = 0;
};

} // namespace scheduler
} // namespace ocs
