/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_system/delay_rebooter.h"

namespace ocs {
namespace system {

DelayRebooter::DelayRebooter(TickType_t delay, IRebooter& rebooter)
    : delay_(delay)
    , rebooter_(rebooter) {
}

void DelayRebooter::reboot() {
    vTaskDelay(delay_);
    rebooter_.reboot();
}

} // namespace system
} // namespace ocs
