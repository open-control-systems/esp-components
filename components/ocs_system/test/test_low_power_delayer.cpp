/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_system/low_power_delayer.h"

namespace ocs {
namespace system {

TEST_CASE("Low power delayer: delay", "[ocs_system], [low_power_delayer]") {
    LowPowerDelayer delayer(core::Second);

    for (unsigned n = 0; n < 10; ++n) {
        delayer.delay(core::Microsecond * 100);
    }
}

} // namespace system
} // namespace ocs
