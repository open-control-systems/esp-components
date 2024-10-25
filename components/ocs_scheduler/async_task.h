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

#include "ocs_scheduler/itask.h"

namespace ocs {
namespace scheduler {

class AsyncTask : public ITask {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handle to post asynchronous events.
    //!  - @p event to post to the event group.
    AsyncTask(EventGroupHandle_t handle, EventBits_t event);

    //! Schedule an event to the event group.
    status::StatusCode run() override;

private:
    const EventBits_t event_ { 0 };

    EventGroupHandle_t handle_ { nullptr };
};

} // namespace scheduler
} // namespace ocs
