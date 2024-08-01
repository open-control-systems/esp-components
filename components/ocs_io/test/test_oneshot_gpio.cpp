/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_io/oneshot_gpio.h"
#include "ocs_test/test_gpio.h"
#include "ocs_test/test_task.h"

namespace ocs {
namespace io {

TEST_CASE("Oneshot GPIO: switch the GPIO: no error", "[ocs_io], [oneshot_gpio]") {
    test::TestTask task(status::StatusCode::OK);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);

    OneshotGpio oneshot_gpio(task, gpio);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, oneshot_gpio.run());
    TEST_ASSERT_TRUE(task.wait());

    TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_off_call_count);
}

TEST_CASE("Oneshot GPIO: switch the GPIO: task error", "[ocs_io], [oneshot_gpio]") {
    test::TestTask task(status::StatusCode::Error);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);

    OneshotGpio oneshot_gpio(task, gpio);

    TEST_ASSERT_EQUAL(status::StatusCode::Error, oneshot_gpio.run());
    TEST_ASSERT_TRUE(task.wait());

    TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_off_call_count);
}

TEST_CASE("Oneshot GPIO: switch the GPIO: task error turn off error",
          "[ocs_io], [oneshot_gpio]") {
    test::TestTask task(status::StatusCode::Error);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::NoData);

    OneshotGpio oneshot_gpio(task, gpio);

    TEST_ASSERT_EQUAL(status::StatusCode::Error, oneshot_gpio.run());
    TEST_ASSERT_TRUE(task.wait());

    TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_off_call_count);
}

TEST_CASE("Oneshot GPIO: switch the GPIO: turn off error", "[ocs_io], [oneshot_gpio]") {
    test::TestTask task(status::StatusCode::OK);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::Error);

    OneshotGpio oneshot_gpio(task, gpio);

    TEST_ASSERT_EQUAL(status::StatusCode::Error, oneshot_gpio.run());
    TEST_ASSERT_TRUE(task.wait());

    TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_off_call_count);
}

TEST_CASE("Oneshot GPIO: switch the GPIO: turn on error", "[ocs_io], [oneshot_gpio]") {
    test::TestTask task(status::StatusCode::OK);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::Error,
                        status::StatusCode::OK);

    OneshotGpio oneshot_gpio(task, gpio);

    TEST_ASSERT_EQUAL(status::StatusCode::Error, oneshot_gpio.run());
    TEST_ASSERT_FALSE(task.wait(pdMS_TO_TICKS(100)));

    TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(0, gpio.turn_off_call_count);
}

} // namespace io
} // namespace ocs
