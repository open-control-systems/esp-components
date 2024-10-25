/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "esp_timer.h"

#include "ocs_system/default_clock.h"

namespace ocs {
namespace system {

core::Time DefaultClock::now() {
    return esp_timer_get_time();
}

} // namespace system
} // namespace ocs
