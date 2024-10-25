/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_scheduler/async_task.h"

namespace ocs {
namespace scheduler {

AsyncTask::AsyncTask(EventGroupHandle_t handle, EventBits_t event)
    : event_(event)
    , handle_(handle) {
}

status::StatusCode AsyncTask::run() {
    xEventGroupSetBits(handle_, event_);
    return status::StatusCode::OK;
}

} // namespace scheduler
} // namespace ocs
