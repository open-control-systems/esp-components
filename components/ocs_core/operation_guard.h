/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

class OperationGuard : public NonCopyable<> {
public:
    //! Suspend FreeRTOS scheduler to ensure a running task won't be preempted.
    //!
    //! @remarks
    //!  After the object construction FreeRTOS API shouldn't be used, since the scheduler
    //!  will be suspended. Any use of FreeRTOS queues, semaphores will lead to the
    //!  application crash. printf() or logging functions are also prohibited, since the
    //!  implementation may use locks or other FreeRTOS primitives.
    OperationGuard();

    //! Resume FreeRTOS scheduler.
    ~OperationGuard();
};

} // namespace core
} // namespace ocs
