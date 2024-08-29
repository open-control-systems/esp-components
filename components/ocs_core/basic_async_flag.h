/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <atomic>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/iasync_flag.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

class BasicAsyncFlag : public IAsyncFlag, public NonCopyable<BasicAsyncFlag> {
public:
    //! Destroy.
    virtual ~BasicAsyncFlag() = default;

    //! Wait for the asynchronous event.
    status::StatusCode wait(TickType_t wait = portMAX_DELAY) override;

protected:
    TaskHandle_t acquire_task_();

private:
    //! Atomicity considerations.
    //!
    //! The implementation represents the producer-consumer model, where the wait() call
    //! is the producer, and the signal() call is the consumer. The consumer can be a
    //! FreeRTOS task or an ISR. The order of operation is only important in the context
    //! of the producer-consumer communication, as a result of which the acquire-release
    //! memory order can be used.
    //!
    //! The consumer acquires the task with the RMW atomic operation, so it should use the
    //! acquire-release memory order. The producer only performs an atomic write
    //! operation, so it should use the release memory order.
    std::atomic<TaskHandle_t> task_to_notify_ { nullptr };
};

} // namespace core
} // namespace ocs
