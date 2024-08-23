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
namespace io {

class IGpio {
public:
    //! Destroy.
    virtual ~IGpio() = default;

    //! Get the GPIO level.
    virtual int get() = 0;

    //! Change the GPIO state to the opposite.
    virtual status::StatusCode flip() = 0;

    //! Enable the GPIO.
    virtual status::StatusCode turn_on() = 0;

    //! Disable the GPIO.
    virtual status::StatusCode turn_off() = 0;
};

} // namespace io
} // namespace ocs
