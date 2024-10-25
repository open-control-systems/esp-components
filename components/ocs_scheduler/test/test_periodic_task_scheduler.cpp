/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <memory>
#include <string>
#include <vector>

#include "unity.h"

#include "ocs_scheduler/constant_delay_estimator.h"
#include "ocs_scheduler/periodic_task_scheduler.h"
#include "ocs_test/test_clock.h"
#include "ocs_test/test_task.h"

namespace ocs {
namespace scheduler {

TEST_CASE("Periodic task scheduler: add task",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const core::Time interval = core::Duration::second;
    const TickType_t delay = pdMS_TO_TICKS(10);
    const char* task_id = "test_task";

    test::TestClock clock;
    clock.value = 42;

    test::TestTask task(status::StatusCode::OK);
    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task, task_id, interval));

    TEST_ASSERT_FALSE(task.was_run_called());

    // The first run is always allowed.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_TRUE(task.was_run_called());
    task.reset(status::StatusCode::OK);

    clock.value += (interval - 1);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_FALSE(task.was_run_called());

    clock.value += 1;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_TRUE(task.was_run_called());
    task.reset(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_FALSE(task.was_run_called());
}

TEST_CASE("Periodic task scheduler: add same task twice",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const TickType_t delay = pdMS_TO_TICKS(10);

    test::TestClock clock;
    test::TestTask task(status::StatusCode::OK);
    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task, "test_task", core::Duration::second));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      task_scheduler.add(task, "test_task", core::Duration::second));
}

TEST_CASE("Periodic task scheduler: add multiple tasks",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const unsigned task_count = 10;
    const TickType_t delay = pdMS_TO_TICKS(10);
    const core::Time interval = core::Duration::second;

    test::TestClock clock;
    clock.value = 42;

    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    using TaskPtr = std::shared_ptr<test::TestTask>;
    using TaskList = std::vector<TaskPtr>;
    TaskList tasks;

    for (unsigned n = 0; n < task_count; ++n) {
        TaskPtr task(new (std::nothrow) test::TestTask(status::StatusCode::OK));
        TEST_ASSERT_NOT_NULL(task);

        tasks.push_back(task);
    }

    for (unsigned n = 0; n < task_count; ++n) {
        const std::string id = "test_task_" + std::to_string(n);
        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          task_scheduler.add(*tasks[n], id.c_str(), interval));
    }

    // The first run is always allowed.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_TRUE(task->was_run_called());
    }
    for (auto& task : tasks) {
        task->reset(status::StatusCode::OK);
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_FALSE(task->was_run_called());
    }

    clock.value += interval;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_TRUE(task->was_run_called());
    }
    for (auto& task : tasks) {
        task->reset(status::StatusCode::OK);
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_FALSE(task->was_run_called());
    }
}

TEST_CASE("Periodic task scheduler: max number of tasks overflow",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const core::Time interval = core::Duration::second;
    const TickType_t delay = pdMS_TO_TICKS(10);

    test::TestClock clock;
    clock.value = 42;

    ConstantDelayEstimator estimator(delay);
    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 1);

    test::TestTask task1(status::StatusCode::OK);
    test::TestTask task2(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task1, "task_1", interval));

    TEST_ASSERT_EQUAL(status::StatusCode::Error,
                      task_scheduler.add(task2, "task_2", interval));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_TRUE(task1.was_run_called());
    TEST_ASSERT_FALSE(task2.was_run_called());

    task1.reset(status::StatusCode::OK);
    task2.reset(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_FALSE(task1.was_run_called());
    TEST_ASSERT_FALSE(task2.was_run_called());
}

} // namespace scheduler
} // namespace ocs
