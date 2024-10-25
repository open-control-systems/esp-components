/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_core/rate_limiter.h"
#include "ocs_test/test_clock.h"

namespace ocs {
namespace core {

TEST_CASE("Rate limiter: allow operation to be performed", "[ocs_core], [rate_limiter]") {
    const Time interval = second;

    test::TestClock clock;
    clock.value = 42;

    RateLimiter limiter(clock, interval);

    // Limiter has just been created and should be allowed to operate.
    TEST_ASSERT_TRUE(limiter.allow());

    // Operation has been performed, no more than one operation per interval is allowed.
    TEST_ASSERT_FALSE(limiter.allow());

    // Update the current time, but do not exceed the rate interval.
    clock.value += interval - core::Duration::microsecond;
    TEST_ASSERT_FALSE(limiter.allow());

    // Again, but now the rate interval has been exceeded.
    clock.value += core::Duration::microsecond;
    TEST_ASSERT_TRUE(limiter.allow());
    TEST_ASSERT_FALSE(limiter.allow());
}

} // namespace core
} // namespace ocs
