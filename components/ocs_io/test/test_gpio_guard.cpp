/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_io/gpio_guard.h"
#include "ocs_test/test_gpio.h"

namespace ocs {
namespace io {

TEST_CASE("GPIO guard: no error", "[ocs_io], [gpio_guard]") {
    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);

    { GpioGuard gpio_guard(gpio); }

    TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_off_call_count);
}

TEST_CASE("GPIO guard: turn off error", "[ocs_io], [gpio_guard]") {
    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::Error);

    { GpioGuard gpio_guard(gpio); }

    TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_off_call_count);
}

TEST_CASE("GPIO guard: turn on error", "[ocs_io], [gpio_guard]") {
    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::Error,
                        status::StatusCode::OK);

    { GpioGuard gpio_guard(gpio); }

    TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_off_call_count);
}

TEST_CASE("GPIO guard: turn on/off errors", "[ocs_io], [gpio_guard]") {
    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::Error,
                        status::StatusCode::Error);

    { GpioGuard gpio_guard(gpio); }

    TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_off_call_count);
}

} // namespace io
} // namespace ocs
