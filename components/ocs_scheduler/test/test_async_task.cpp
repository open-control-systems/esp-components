/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_core/static_event_group.h"
#include "ocs_scheduler/async_task.h"
#include "ocs_status/code.h"

namespace ocs {
namespace scheduler {

TEST_CASE("Async task: run sets event in event group", "[ocs_scheduler], [async_task]") {
    core::StaticEventGroup event_group;

    const EventBits_t event = BIT(0);
    AsyncTask task(event_group.get(), event);

    // Initially, the event should not be set.
    TEST_ASSERT_EQUAL(0, xEventGroupGetBits(event_group.get()));

    // Run the task, which should set the event in the event group.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task.run());

    // Check if the event bit is now set.
    TEST_ASSERT_EQUAL(event, xEventGroupGetBits(event_group.get()));
}

TEST_CASE("Async task: multiple runs set event correctly",
          "[ocs_scheduler], [async_task]") {
    core::StaticEventGroup event_group;

    const EventBits_t event1 = BIT(0);
    const EventBits_t event2 = BIT(1);

    AsyncTask task1(event_group.get(), event1);
    AsyncTask task2(event_group.get(), event2);

    // Run the first task and check that the first event is set.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task1.run());
    TEST_ASSERT_EQUAL(event1, xEventGroupGetBits(event_group.get()));

    // Run the second task and check that both events are now set.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task2.run());
    TEST_ASSERT_EQUAL(event1 | event2, xEventGroupGetBits(event_group.get()));
}

} // namespace scheduler
} // namespace ocs
