/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "ocs_status/code.h"

namespace ocs {
namespace scheduler {

class ITask {
public:
    //! Event type used for the events posting.
    using Event = EventBits_t;

    //! Destroy.
    virtual ~ITask() = default;

    //! Run a task.
    virtual status::StatusCode run() = 0;
};

} // namespace scheduler
} // namespace ocs
