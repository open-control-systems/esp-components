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

//! Resource locker/unlocker.
class ILocker {
public:
    //! Destroy.
    virtual ~ILocker() = default;

    //! Lock the resource.
    virtual status::StatusCode lock(TickType_t wait = portMAX_DELAY) = 0;

    //! Unlock the resource.
    virtual status::StatusCode unlock() = 0;
};

} // namespace core
} // namespace ocs
