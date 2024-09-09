/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "ocs_core/future.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_recursive_mutex.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace scheduler {

class AsyncFuncScheduler : public ITask, public core::NonCopyable<> {
public:
    using FuturePtr = std::shared_ptr<core::Future>;
    using Func = std::function<status::StatusCode()>;

    //! Initialize.
    //!
    //! @params
    //!  - @p max_event_count - maximum number of asynchronous events that can be enqueued
    //!    before run() is called.
    explicit AsyncFuncScheduler(unsigned max_event_count);

    //! Run scheduled events.
    status::StatusCode run() override;

    //! Add @p func to be executed asynchronously.
    //!
    //! @remarks
    //!  It is safe to call scheduler functions in @p func.
    FuturePtr add(Func func);

private:
    const unsigned max_event_count_ { 0 };

    using Queue = std::vector<Func>;
    using QueuePtr = std::shared_ptr<Queue>;

    core::StaticRecursiveMutex mu_;
    QueuePtr read_queue_;
    QueuePtr write_queue_;
};

} // namespace scheduler
} // namespace ocs
