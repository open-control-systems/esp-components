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

#include "ocs_core/noncopyable.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace system {

class DelayRebooter : public IRebooter, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p delay to wait before the actual reboot is happened.
    //!  - @p rebooter to perform the actual reboot.
    DelayRebooter(TickType_t delay, IRebooter& rebooter);

    //! Wait extra delay and then reboot.
    void reboot() override;

private:
    const TickType_t delay_ { 0 };

    IRebooter& rebooter_;
};

} // namespace system
} // namespace ocs
