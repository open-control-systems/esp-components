/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/basic_static_mutex.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

//! FreeRTOS mutex to be used in ISR.
class StaticMutexIsr : public BasicStaticMutex, public NonCopyable<> {
public:
    //! Lock the mutex.
    status::StatusCode lock(TickType_t wait = portMAX_DELAY) override;

    //! Unlock the mutex.
    status::StatusCode unlock() override;
};

} // namespace core
} // namespace ocs
