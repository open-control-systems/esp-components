/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_scheduler/fanout_task.h"

namespace ocs {
namespace scheduler {

status::StatusCode FanoutTask::run() {
    for (auto& task : tasks_) {
        const auto code = task->run();
        if (code != status::StatusCode::OK) {
            return code;
        }
    }

    return status::StatusCode::OK;
}

void FanoutTask::add(ITask& task) {
    tasks_.emplace_back(&task);
}

} // namespace scheduler
} // namespace ocs
