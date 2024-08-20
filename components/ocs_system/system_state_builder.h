/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace system {

class SystemStateBuilder : public core::NonCopyable<> {
public:
    struct State {
        configRUN_TIME_COUNTER_TYPE total_time { 0 };
        std::vector<TaskStatus_t> states;
    };

    //! Initialize.
    SystemStateBuilder();

    //! Return the state of each task in the system.
    const State& get() const;

private:
    State state_;
};

} // namespace system
} // namespace ocs
