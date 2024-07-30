/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace scheduler {

class ITimer {
public:
    //! Destroy.
    virtual ~ITimer() = default;

    //! Start the timer.
    virtual status::StatusCode start() = 0;

    //! Stop the timer.
    virtual status::StatusCode stop() = 0;
};

} // namespace scheduler
} // namespace ocs
