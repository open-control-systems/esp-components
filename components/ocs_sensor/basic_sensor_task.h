/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace sensor {

template <typename T>
class BasicSensorTask : public scheduler::ITask,
                        public core::NonCopyable<BasicSensorTask<T>> {
public:
    //! Destroy.
    virtual ~BasicSensorTask() = default;

    //! Schedule asynchronous events to the underlying sensor.
    status::StatusCode run() override {
        return async_task_->run();
    }

    //! Return the underlying sensor.
    T& get_sensor() {
        return *sensor_;
    }

protected:
    std::unique_ptr<T> sensor_;

    scheduler::ITask* async_task_ { nullptr };
    std::unique_ptr<scheduler::ITimer> async_task_timer_;
};

} // namespace sensor
} // namespace ocs
