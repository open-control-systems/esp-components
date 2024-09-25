/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/iclock.h"
#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/icounter.h"
#include "ocs_diagnostic/persistent_counter.h"
#include "ocs_diagnostic/time_counter.h"
#include "ocs_scheduler/itask.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/ireboot_handler.h"

namespace ocs {
namespace diagnostic {

class StateCounter : public ICounter,
                     public system::IRebootHandler,
                     public scheduler::ITask,
                     public core::NonCopyable<> {
public:
    using State = uint8_t;

    //! Initialize.
    //!
    //! @params
    //!  - @p storage to store the state interval.
    //!  - @p clock to count the state interval.
    //!  - @p id - human readable counter identifier.
    //!  - @p resolution - time resolution for the counting value.
    //!  - @p required_state - to count the time in this state, can't be zero.
    //!
    //! @remarks
    //!  All operations should be scheduled on the same task scheduler.
    StateCounter(storage::IStorage& storage,
                 core::IClock& clock,
                 const char* id,
                 core::microseconds_t resolution,
                 State required_state);

    //! Update the counter state.
    //!
    //! @notes
    //!  - If the counter is updated with the required state, configured in a constructor,
    //!    it invalidates previously saved value, and starts counting from the beginning.
    //!
    //!  - If the counter losts the required state, it saved the current counter value,
    //!    and start returning this value, instead of the value from the underlying
    //!    counters.
    void update(State state);

    //! Return the counter identifier.
    const char* id() const override;

    //! Return the counting value from the underlying counters if required state is set.
    ICounter::Value get() const override;

    //! Persist counter values on reboot.
    void handle_reboot() override;

    //! Save counter value in a persistent storage if the required state is set.
    status::StatusCode run() override;

private:
    void handle_state_set_();
    void handle_state_lost_();

    const State required_state_ { 0 };

    bool alive_ { false };
    State current_state_ { 0 };
    ICounter::Value last_value_ { 0 };

    std::unique_ptr<TimeCounter> time_counter_;
    std::unique_ptr<PersistentCounter> persistent_counter_;

    ICounter* counter_ { nullptr };
};

} // namespace diagnostic
} // namespace ocs
