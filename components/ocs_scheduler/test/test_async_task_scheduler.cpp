/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <memory>
#include <vector>

#include "unity.h"

#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_scheduler/constant_delay_estimator.h"
#include "ocs_test/test_task.h"

namespace ocs {
namespace scheduler {

namespace {

void wait_task(ITaskScheduler& scheduler, test::TestTask& task) {
    while (true) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.run());

        if (task.wait(pdMS_TO_TICKS(30)) == status::StatusCode::OK) {
            break;
        }
    }
}

} // namespace

TEST_CASE("Async task scheduler: wait for events",
          "[ocs_scheduler], [async_task_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(portMAX_DELAY);
    AsyncTaskScheduler scheduler(estimator, scheduler_id);

    test::TestTask task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        scheduler.add(task, "test_task", core::Duration::millisecond * 100));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.run());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.stop());

    TEST_ASSERT_TRUE(task.was_run_called());
}

TEST_CASE("Async task scheduler: register same task multiple times",
          "[ocs_scheduler], [async_task_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(portMAX_DELAY);
    AsyncTaskScheduler scheduler(estimator, scheduler_id);

    test::TestTask task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        scheduler.add(task, "test_task", core::Duration::millisecond * 100));
    TEST_ASSERT_EQUAL(
        status::StatusCode::InvalidArg,
        scheduler.add(task, "test_task", core::Duration::millisecond * 100));
}

TEST_CASE("Async task scheduler: register maximum tasks",
          "[ocs_scheduler], [async_task_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(pdMS_TO_TICKS(30));
    AsyncTaskScheduler scheduler(estimator, scheduler_id);

    using TaskPtr = std::shared_ptr<test::TestTask>;
    std::vector<TaskPtr> tasks;

    for (unsigned n = 0; n < scheduler.max_count(); ++n) {
        TaskPtr task(new (std::nothrow) test::TestTask(status::StatusCode::OK));
        TEST_ASSERT_NOT_NULL(task);

        tasks.push_back(task);
    }

    for (unsigned n = 0; n < tasks.size(); ++n) {
        const std::string task_id = std::string("test_task_") + std::to_string(n);

        TEST_ASSERT_EQUAL(
            status::StatusCode::OK,
            scheduler.add(*tasks[n], task_id.c_str(), core::Duration::millisecond * 30));
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.start());

    for (auto& task : tasks) {
        wait_task(scheduler, *task);
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.stop());
}

TEST_CASE("Async task scheduler: register maximum tasks: some failed",
          "[ocs_scheduler], [async_task_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(pdMS_TO_TICKS(30));
    AsyncTaskScheduler scheduler(estimator, scheduler_id);

    using TaskPtr = std::shared_ptr<test::TestTask>;
    std::vector<TaskPtr> tasks;

    for (unsigned n = 0; n < scheduler.max_count(); ++n) {
        status::StatusCode code = status::StatusCode::OK;
        if (n % 2 == 0) {
            code = status::StatusCode::Error;
        }

        TaskPtr task(new (std::nothrow) test::TestTask(code));
        TEST_ASSERT_NOT_NULL(task);

        tasks.push_back(task);
    }

    for (unsigned n = 0; n < tasks.size(); ++n) {
        const std::string task_id = std::string("test_task_") + std::to_string(n);

        TEST_ASSERT_EQUAL(
            status::StatusCode::OK,
            scheduler.add(*tasks[n], task_id.c_str(), core::Duration::millisecond * 30));
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.start());

    for (auto& task : tasks) {
        wait_task(scheduler, *task);
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.stop());
}

TEST_CASE("Async task scheduler: register tasks overflow",
          "[ocs_scheduler], [async_task_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(pdMS_TO_TICKS(30));
    AsyncTaskScheduler scheduler(estimator, scheduler_id);

    using TaskPtr = std::shared_ptr<test::TestTask>;
    std::vector<TaskPtr> tasks;

    for (unsigned n = 0; n < scheduler.max_count(); ++n) {
        TaskPtr task(new (std::nothrow) test::TestTask(status::StatusCode::OK));
        TEST_ASSERT_NOT_NULL(task);

        tasks.push_back(task);
    }

    for (unsigned n = 0; n < tasks.size(); ++n) {
        const std::string task_id = std::string("test_task_") + std::to_string(n);

        TEST_ASSERT_EQUAL(
            status::StatusCode::OK,
            scheduler.add(*tasks[n], task_id.c_str(), core::Duration::millisecond * 30));
    }

    test::TestTask task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::Error,
                      scheduler.add(task, "test_task", core::Duration::millisecond * 10));
}

} // namespace scheduler
} // namespace ocs
