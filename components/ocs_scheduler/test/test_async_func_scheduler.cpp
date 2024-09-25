/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_scheduler/async_func_scheduler.h"

namespace ocs {
namespace scheduler {

TEST_CASE("Async func scheduler: no events", "[ocs_scheduler], [async_func_scheduler]") {
    AsyncFuncScheduler func_scheduler(1);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, func_scheduler.run());
}

TEST_CASE("Async func scheduler: number events overflow",
          "[ocs_scheduler], [async_func_scheduler]") {
    AsyncFuncScheduler func_scheduler(1);

    auto future1 = func_scheduler.add([]() {
        vTaskDelay(pdMS_TO_TICKS(50));
        return status::StatusCode::IsrYield;
    });
    TEST_ASSERT_NOT_NULL(future1);

    auto future2 = func_scheduler.add([]() {
        vTaskDelay(pdMS_TO_TICKS(50));
        return status::StatusCode::OK;
    });
    TEST_ASSERT_NULL(future2);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, func_scheduler.run());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future1->wait());
    TEST_ASSERT_EQUAL(status::StatusCode::IsrYield, future1->code());

    future2 = func_scheduler.add([]() {
        vTaskDelay(pdMS_TO_TICKS(50));
        return status::StatusCode::NoData;
    });
    TEST_ASSERT_NOT_NULL(future2);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, func_scheduler.run());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future2->wait());
    TEST_ASSERT_EQUAL(status::StatusCode::NoData, future2->code());
}

TEST_CASE("Async func scheduler: events one by one",
          "[ocs_scheduler], [async_func_scheduler]") {
    AsyncFuncScheduler func_scheduler(1);

    auto future1 = func_scheduler.add([]() {
        vTaskDelay(pdMS_TO_TICKS(50));
        return status::StatusCode::IsrYield;
    });
    TEST_ASSERT_NOT_NULL(future1);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, func_scheduler.run());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future1->wait());
    TEST_ASSERT_EQUAL(status::StatusCode::IsrYield, future1->code());

    auto future2 = func_scheduler.add([]() {
        vTaskDelay(pdMS_TO_TICKS(50));
        return status::StatusCode::Timeout;
    });
    TEST_ASSERT_EQUAL(status::StatusCode::OK, func_scheduler.run());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future2->wait());
    TEST_ASSERT_EQUAL(status::StatusCode::Timeout, future2->code());
}

TEST_CASE("Async func scheduler: reschedule event",
          "[ocs_scheduler], [async_func_scheduler]") {
    AsyncFuncScheduler func_scheduler(1);

    AsyncFuncScheduler::FuturePtr future1;
    AsyncFuncScheduler::FuturePtr future2;

    future1 = func_scheduler.add([&func_scheduler, &future2]() {
        future2 = func_scheduler.add([]() {
            return status::StatusCode::Timeout;
        });

        return status::StatusCode::IsrYield;
    });
    TEST_ASSERT_NOT_NULL(future1);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, func_scheduler.run());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future1->wait());
    TEST_ASSERT_EQUAL(status::StatusCode::IsrYield, future1->code());

    TEST_ASSERT_NOT_NULL(future2);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, func_scheduler.run());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future2->wait());
    TEST_ASSERT_EQUAL(status::StatusCode::Timeout, future2->code());
}

} // namespace scheduler
} // namespace ocs
