/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/ilocker.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

//! Static FreeRTOS mutex.
class BasicStaticMutex : public ILocker, public NonCopyable<BasicStaticMutex> {
public:
    //! Initialize.
    BasicStaticMutex();

    //! Destroy.
    virtual ~BasicStaticMutex() = default;

protected:
    SemaphoreHandle_t sem_ { nullptr };

private:
    StaticSemaphore_t buff_;
};

} // namespace core
} // namespace ocs
