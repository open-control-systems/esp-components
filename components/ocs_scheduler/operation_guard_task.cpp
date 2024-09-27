/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_scheduler/operation_guard_task.h"
#include "ocs_core/operation_guard.h"

namespace ocs {
namespace scheduler {

OperationGuardTask::OperationGuardTask(ITask& task)
    : task_(task) {
}

status::StatusCode OperationGuardTask::run() {
    core::OperationGuard operation_guard;

    return task_.run();
}

} // namespace scheduler
} // namespace ocs
