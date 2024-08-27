/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "freertos/FreeRTOS.h"

#include "ocs_status/code.h"

namespace ocs {
namespace core {

class IAsyncFlag {
public:
    //! Destroy.
    virtual ~IAsyncFlag() = default;

    //! Wait for the event.
    virtual status::StatusCode wait(TickType_t wait = portMAX_DELAY) = 0;

    //! Signal the event.
    virtual status::StatusCode signal() = 0;
};

} // namespace core
} // namespace ocs
