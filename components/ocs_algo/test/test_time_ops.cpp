/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "unity.h"

#include "ocs_algo/time_ops.h"
#include "ocs_core/time.h"

namespace ocs {
namespace algo {

TEST_CASE("Time ops: after: check equal", "[ocs_algo], [time_ops]") {
    const core::Time start = 237;
    const core::Time interval = 331;
    const core::Time now = start + interval;

    TEST_ASSERT_TRUE(TimeOps::after(start, now, interval));
}

TEST_CASE("Time ops: after: check less", "[ocs_algo], [time_ops]") {
    const core::Time start = 237;
    const core::Time interval = 331;
    const core::Time now = start + interval - 1;

    TEST_ASSERT_FALSE(TimeOps::after(start, now, interval));
}

TEST_CASE("Time ops: after: check greater", "[ocs_algo], [time_ops]") {
    const core::Time start = 237;
    const core::Time interval = 331;
    const core::Time now = start + interval + 1;

    TEST_ASSERT_TRUE(TimeOps::after(start, now, interval));
}

TEST_CASE("Time ops: after: check back in time", "[ocs_algo], [time_ops]") {
    { // check signed
        const core::Time start = -123;
        const core::Time interval = 321;
        const core::Time now = start + interval;

        TEST_ASSERT_TRUE(TimeOps::after(start, now, interval));
    }

    { // check unsigned
        const TickType_t start = pdMS_TO_TICKS(0) - pdMS_TO_TICKS(123);
        const TickType_t interval = pdMS_TO_TICKS(321);
        const TickType_t now = start + interval;

        TEST_ASSERT_TRUE(TimeOps::after(start, now, interval));
    }
}

} // namespace algo
} // namespace ocs
