/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itimer.h"
#include "ocs_status/code.h"

namespace ocs {
namespace scheduler {

class TimerStore : public core::NonCopyable<> {
public:
    //! Register a new timer.
    void add(ITimer& timer);

    //! Start all timers.
    status::StatusCode start();

private:
    std::vector<ITimer*> timers_;
};

} // namespace scheduler
} // namespace ocs
