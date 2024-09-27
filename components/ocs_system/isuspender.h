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

class ISuspender {
public:
    //! Destroy.
    virtual ~ISuspender() = default;

    //! Suspend the system.
    virtual status::StatusCode suspend() = 0;

    //! Resume the system.
    virtual status::StatusCode resume() = 0;
};

} // namespace system
} // namespace ocs
