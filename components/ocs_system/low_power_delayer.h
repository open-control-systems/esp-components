/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/async_flag_isr.h"
#include "ocs_system/basic_delayer.h"

namespace ocs {
namespace system {

class LowPowerDelayer : public BasicDelayer, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p resolution - single tick duration equals to 1/resolution.
    explicit LowPowerDelayer(core::Time resolution);

    //! Delay based on the hardware timers and interrupts.
    //!
    //! @remarks
    //!  The delay can't be considered very accurate, because the calling task waits for
    //!  the delay in a low-power way, the CPU is not kept busy in a loop. As a result, it
    //!  takes some time for the FreeRTOS scheduler to return the execution to the calling
    //!  task.
    status::StatusCode delay(core::Time delay) override;

private:
    static bool handle_alarm_callback_(gptimer_handle_t handle,
                                       const gptimer_alarm_event_data_t* edata,
                                       void* arg);

    core::AsyncFlagIsr flag_;
};

} // namespace system
} // namespace ocs
