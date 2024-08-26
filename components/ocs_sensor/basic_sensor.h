/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <atomic>

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace sensor {

template <typename T>
class BasicSensor : public scheduler::ITask, public core::NonCopyable<BasicSensor<T>> {
public:
    //! Destroy.
    virtual ~BasicSensor() = default;

    //! Return the underlying sensor data.
    T get_data() const {
        return data_.load(std::memory_order_acquire);
    }

protected:
    void set_data_(const T& data) {
        data_.store(data, std::memory_order_release);
    }

private:
    //! Atomicity considerations.
    //!
    //! The implementation represents the single producer multiple consumers model (SPMC),
    //! where the set_data_() call is the producer, and the get_data() call is the
    //! consumer. The consumer can be a FreeRTOS task or an ISR. The order of operation is
    //! only important in the context of the producer-consumer communication, as a result
    //! of which the acquire-release memory order can be used.
    //!
    //! The consumer acquires the data with the read atomic operation, so it should use
    //! the acquire memory order. The producer only performs an atomic write operation, so
    //! it should use the release memory order.
    std::atomic<T> data_;
};

} // namespace sensor
} // namespace ocs
