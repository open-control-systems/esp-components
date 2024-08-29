/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "freertos/FreeRTOS.h"

#include "ocs_core/ilocker.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

//! RAII resource locker.
class LockGuard : public NonCopyable<> {
public:
    //! Acquire the resource.
    //!
    //! @params
    //!  - @p locker to lock/unlock the resource.
    //!  - @p wait - interval to wait for the resource to become available.
    LockGuard(ILocker& locker, TickType_t wait = portMAX_DELAY);

    //! Release the resource.
    ~LockGuard();

private:
    ILocker& locker_;
};

} // namespace core
} // namespace ocs
