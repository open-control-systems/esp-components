/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <mutex>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

//! Static FreeRTOS mutex.
class StaticMutex : public NonCopyable<> {
public:
    //! RAII lock.
    using Lock = std::lock_guard<StaticMutex>;

    //! Initialize.
    StaticMutex();

    //! Lock mutex.
    void lock();

    //! Unlock mutex.
    void unlock();

private:
    StaticSemaphore_t buff_;
    SemaphoreHandle_t sem_ { nullptr };
};

} // namespace core
} // namespace ocs
