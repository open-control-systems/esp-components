/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_test/test_task.h"

namespace ocs {
namespace test {

TestTask::TestTask(status::StatusCode code)
    : cond_(mu_)
    , code_(code) {
}

status::StatusCode TestTask::run() {
    core::StaticMutex::Lock lock(mu_);

    run_called_ = true;
    cond_.broadcast();

    return code_;
}

bool TestTask::was_run_called() const {
    core::StaticMutex::Lock lock(mu_);

    return run_called_;
}

bool TestTask::wait(TickType_t wait) {
    core::StaticMutex::Lock lock(mu_);

    while (!run_called_) {
        if (!cond_.wait(wait)) {
            return false;
        }
    }

    return true;
}

} // namespace test
} // namespace ocs
