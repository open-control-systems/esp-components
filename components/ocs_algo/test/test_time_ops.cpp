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

TEST_CASE("Time ops: parse time: valid input", "[ocs_algo], [time_ops]") {
    const char* str = "1640295065";
    const auto timestamp = TimeOps::parse_time(str);
    TEST_ASSERT_TRUE(timestamp.has_value());
    TEST_ASSERT_EQUAL(timestamp.value(), 1640295065);
}

TEST_CASE("Time ops: parse time: invalid input", "[ocs_algo], [time_ops]") {
    const char* str = "123Invalid string";
    const auto timestamp = TimeOps::parse_time(str);
    TEST_ASSERT_FALSE(timestamp.has_value());
}

TEST_CASE("Time ops: parse time: empty input", "[ocs_algo], [time_ops]") {
    const char* str = "";
    const auto timestamp = TimeOps::parse_time(str);
    TEST_ASSERT_FALSE(timestamp.has_value());
}

TEST_CASE("Time ops: parse time: overflow", "[ocs_algo], [time_ops]") {
    const char* str = "144029506500000";
    const auto timestamp = TimeOps::parse_time(str);
    TEST_ASSERT_FALSE(timestamp.has_value());
}

} // namespace algo
} // namespace ocs
