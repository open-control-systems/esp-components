/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_scheduler/timer_store.h"
#include "ocs_status/code.h"
#include "ocs_test/test_timer.h"

namespace ocs {
namespace scheduler {

TEST_CASE("Timer store: start all timers successfully",
          "[ocs_scheduler], [timer_store]") {
    TimerStore store;

    test::TestTimer timer1(status::StatusCode::OK);
    test::TestTimer timer2(status::StatusCode::OK);

    store.add(timer1);
    store.add(timer2);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.start());
    TEST_ASSERT_EQUAL(1, timer1.start_call_count);
    TEST_ASSERT_EQUAL(1, timer2.start_call_count);
}

TEST_CASE("Timer store: start fails on first timer", "[ocs_scheduler], [timer_store]") {
    TimerStore store;

    test::TestTimer timer1(status::StatusCode::Error);
    test::TestTimer timer2(status::StatusCode::OK);

    store.add(timer1);
    store.add(timer2);

    TEST_ASSERT_EQUAL(status::StatusCode::Error, store.start());
    TEST_ASSERT_EQUAL(1, timer1.start_call_count);
    TEST_ASSERT_EQUAL(0, timer2.start_call_count);
}

TEST_CASE("Timer store: start fails on second timer", "[ocs_scheduler], [timer_store]") {
    TimerStore store;

    test::TestTimer timer1(status::StatusCode::OK);
    test::TestTimer timer2(status::StatusCode::Error);

    store.add(timer1);
    store.add(timer2);

    TEST_ASSERT_EQUAL(status::StatusCode::Error, store.start());
    TEST_ASSERT_EQUAL(1, timer1.start_call_count);
    TEST_ASSERT_EQUAL(1, timer2.start_call_count);
}

TEST_CASE("Timer store: no timers added", "[ocs_scheduler], [timer_store]") {
    TimerStore store;

    TEST_ASSERT_EQUAL(status::StatusCode::Error, store.start());
}

} // namespace scheduler
} // namespace ocs
