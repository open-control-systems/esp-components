/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/fanout_task.h"
#include "ocs_test/test_task.h"

namespace ocs {
namespace scheduler {

TEST_CASE("Task fanout: run all tasks successfully", "[ocs_scheduler], [fanout_task]") {
    FanoutTask fanout;

    test::TestTask task1(status::StatusCode::OK);
    test::TestTask task2(status::StatusCode::OK);

    fanout.add(task1);
    fanout.add(task2);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fanout.run());
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(1, task2.run_call_count());
}

TEST_CASE("Task fanout: run fails on first task", "[ocs_scheduler], [fanout_task]") {
    FanoutTask fanout;

    test::TestTask task1(status::StatusCode::Error);
    test::TestTask task2(status::StatusCode::OK);

    fanout.add(task1);
    fanout.add(task2);

    TEST_ASSERT_EQUAL(status::StatusCode::Error, fanout.run());
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());
}

TEST_CASE("Task fanout: run fails on second task", "[ocs_scheduler], [fanout_task]") {
    FanoutTask fanout;

    test::TestTask task1(status::StatusCode::OK);
    test::TestTask task2(status::StatusCode::Error);

    fanout.add(task1);
    fanout.add(task2);

    TEST_ASSERT_EQUAL(status::StatusCode::Error, fanout.run());
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(1, task2.run_call_count());
}

TEST_CASE("Task fanout: no tasks added", "[ocs_scheduler], [fanout_task]") {
    FanoutTask fanout;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fanout.run());
}

} // namespace scheduler
} // namespace ocs
