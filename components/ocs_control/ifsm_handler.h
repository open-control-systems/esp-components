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
namespace control {

class IFsmHandler {
public:
    //! Destroy.
    virtual ~IFsmHandler() = default;

    //! Handle a FSM state.
    virtual status::StatusCode handle_state() = 0;

    //! Handle a FSM state transition.
    virtual status::StatusCode handle_transit() = 0;
};

} // namespace control
} // namespace ocs
