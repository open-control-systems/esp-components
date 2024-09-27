/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace scheduler {

class OperationGuardTask : public ITask, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p task which operations shouldn't be preempted.
    //!
    //! @remarks
    //!  Task shouldn't use any FreeRTOS primitives, for more details
    //!  see core::OperationGuard.
    explicit OperationGuardTask(ITask& task);

    //! Ensure the underlying task won't be preempted.
    status::StatusCode run() override;

private:
    ITask& task_;
};

} // namespace scheduler
} // namespace ocs
