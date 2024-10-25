/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "esp_timer.h"

#include "ocs_core/noncopyable.h"
#include "ocs_core/time.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itimer.h"

namespace ocs {
namespace scheduler {

class HighResolutionTimer : public ITimer, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p task to be invoked periodically at the configured interval.
    //!  - @p name to distinguish one timer from another.
    //!  - @p interval - timer interval.
    HighResolutionTimer(ITask& task, const char* name, core::Time interval);

    //! Destroy timer.
    ~HighResolutionTimer();

    //! Start ESP high-resolution timer.
    status::StatusCode start() override;

    //! Stop ESP high-resolution timer.
    status::StatusCode stop() override;

private:
    static void handle_cb_(void* arg);

    const core::Time interval_ { 0 };

    ITask& task_;

    esp_timer_handle_t handle_ { nullptr };
};

} // namespace scheduler
} // namespace ocs
