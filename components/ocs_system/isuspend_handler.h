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
namespace system {

//! At some point of time the system may need more resources, than it currently has.
//! In this case it can ask the implementation of this interface to release some memory
//! or CPU usage, thus allowing the system to perform some heavy operations.
class ISuspendHandler {
public:
    //! Destroy.
    virtual ~ISuspendHandler() = default;

    //! Suspend the component.
    //!
    //! @notes
    //!  It is a good place to release memory and CPU usage.
    virtual status::StatusCode handle_suspend() = 0;

    //! Resume the component.
    virtual status::StatusCode handle_resume() = 0;
};

} // namespace system
} // namespace ocs
