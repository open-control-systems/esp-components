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
#include "ocs_test/test_task.h"

namespace ocs {
namespace scheduler {

TEST_CASE("Async task scheduler: wait for events",
          "[ocs_scheduler], [async_task_scheduler]") {
    AsyncTaskScheduler scheduler;
    test::TestTask task(status::StatusCode::OK);

    auto async_task = scheduler.add(task);
    TEST_ASSERT_NOT_NULL(async_task);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, async_task->run());

    scheduler.wait();
    TEST_ASSERT_TRUE(task.was_run_called());
}

TEST_CASE("Async task scheduler: register same task multiple times",
          "[ocs_scheduler], [async_task_scheduler]") {
    AsyncTaskScheduler scheduler;
    test::TestTask task(status::StatusCode::OK);

    auto async_task1 = scheduler.add(task);
    auto async_task2 = scheduler.add(task);
    TEST_ASSERT_NOT_NULL(async_task1);
    TEST_ASSERT_NULL(async_task2);
}

TEST_CASE("Async task scheduler: register maximum tasks",
          "[ocs_scheduler], [async_task_scheduler]") {
    AsyncTaskScheduler scheduler;

    using TaskPtr = std::shared_ptr<test::TestTask>;
    std::vector<TaskPtr> tasks;

    for (unsigned n = 0; n < AsyncTaskScheduler::max_task_count; ++n) {
        TaskPtr task(new (std::nothrow) test::TestTask(status::StatusCode::OK));
        TEST_ASSERT_NOT_NULL(task);

        tasks.push_back(task);
    }

    std::vector<ITask*> async_tasks;

    for (auto& task : tasks) {
        auto async_task = scheduler.add(*task);
        TEST_ASSERT_NOT_NULL(async_task);

        async_tasks.push_back(async_task);
    }

    for (auto& async_task : async_tasks) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, async_task->run());
    }

    scheduler.wait();

    for (auto& task : tasks) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task->wait());
    }
}

TEST_CASE("Async task scheduler: register maximum tasks: some failed",
          "[ocs_scheduler], [async_task_scheduler]") {
    AsyncTaskScheduler scheduler;

    using TaskPtr = std::shared_ptr<test::TestTask>;
    std::vector<TaskPtr> tasks;

    for (unsigned n = 0; n < AsyncTaskScheduler::max_task_count; ++n) {
        status::StatusCode code = status::StatusCode::OK;
        if (n % 2 == 0) {
            code = status::StatusCode::Error;
        }

        TaskPtr task(new (std::nothrow) test::TestTask(code));
        TEST_ASSERT_NOT_NULL(task);

        tasks.push_back(task);
    }

    std::vector<ITask*> async_tasks;

    for (auto& task : tasks) {
        auto async_task = scheduler.add(*task);
        TEST_ASSERT_NOT_NULL(async_task);

        async_tasks.push_back(async_task);
    }

    for (auto& async_task : async_tasks) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, async_task->run());
    }

    scheduler.wait();

    for (auto& task : tasks) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task->wait());
    }
}

TEST_CASE("Async task scheduler: register tasks overflow",
          "[ocs_scheduler], [async_task_scheduler]") {
    AsyncTaskScheduler scheduler;

    using TaskPtr = std::shared_ptr<ITask>;
    std::vector<TaskPtr> tasks;

    for (unsigned n = 0; n < AsyncTaskScheduler::max_task_count; ++n) {
        TaskPtr task(new (std::nothrow) test::TestTask(status::StatusCode::OK));
        TEST_ASSERT_NOT_NULL(task);
        tasks.push_back(task);
    }

    for (auto& task : tasks) {
        auto async_task = scheduler.add(*task);
        TEST_ASSERT_NOT_NULL(async_task);
    }

    test::TestTask task(status::StatusCode::OK);

    auto async_task = scheduler.add(task);
    TEST_ASSERT_NULL(async_task);
}

} // namespace scheduler
} // namespace ocs
