/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_diagnostic/state_counter.h"
#include "ocs_test/test_clock.h"
#include "ocs_test/test_counter_storage.h"

namespace ocs {
namespace diagnostic {

TEST_CASE("State counter: state is set", "[ocs_diagnostic], [state_counter]") {
    const char* id = "foo";
    const core::microseconds_t resolution = core::Second;

    const unsigned persisted_value = 24;

    test::TestCounterStorage storage;
    storage.set(id, persisted_value);

    const unsigned new_value = 42;

    test::TestClock clock;
    clock.value = new_value * resolution;

    const StateCounter::State required_state = 2;
    const StateCounter::State new_state = required_state;

    StateCounter counter(storage, clock, id, resolution, required_state);

    TEST_ASSERT_TRUE(storage.get(id));

    // There was no previous state, ensure previously saved data is kept.
    TEST_ASSERT_EQUAL(persisted_value, counter.get());
    counter.update(new_state);

    TEST_ASSERT_TRUE(storage.get(id));
    TEST_ASSERT_EQUAL(persisted_value + new_value, counter.get());

    clock.value += new_value * resolution;
    TEST_ASSERT_EQUAL(persisted_value + (2 * new_value), counter.get());
}

TEST_CASE("State counter: state is lost", "[ocs_diagnostic], [state_counter]") {
    const char* id = "foo";

    const core::microseconds_t resolution = core::Second;

    const StateCounter::State required_state = 1;
    const StateCounter::State new_state = 2;

    const unsigned current_value = 25;

    test::TestClock clock;
    clock.value = current_value * resolution;

    test::TestCounterStorage storage;
    TEST_ASSERT_FALSE(storage.get(id));

    StateCounter counter(storage, clock, id, resolution, required_state);

    TEST_ASSERT_FALSE(storage.get(id));
    TEST_ASSERT_EQUAL(0, counter.get());

    // There was no previous state, ensure no data.
    counter.update(new_state);

    TEST_ASSERT_FALSE(storage.get(id));
    TEST_ASSERT_EQUAL(0, counter.get());

    clock.value += current_value * resolution;
    TEST_ASSERT_EQUAL(0, counter.get());
}

TEST_CASE("State counter: state changed multiple times",
          "[ocs_diagnostic], [state_counter]") {
    const char* id = "foo";
    const core::microseconds_t resolution = core::Second;

    const unsigned persisted_value = 24;

    test::TestCounterStorage storage;
    storage.set(id, persisted_value);

    unsigned current_value = 42;

    test::TestClock clock;
    clock.value = current_value * resolution;

    const StateCounter::State required_state = 2;
    const StateCounter::State state_set = required_state;
    const StateCounter::State state_lost = required_state + 1;

    StateCounter counter(storage, clock, id, resolution, required_state);

    // There is no previous state.
    TEST_ASSERT_TRUE(storage.get(id));
    TEST_ASSERT_EQUAL(persisted_value, counter.get());
    counter.update(state_set);

    TEST_ASSERT_TRUE(storage.get(id));
    TEST_ASSERT_EQUAL(persisted_value + current_value, counter.get());

    current_value *= 2;

    clock.value = current_value * resolution;
    TEST_ASSERT_EQUAL(persisted_value + current_value, counter.get());

    counter.update(state_lost);

    const auto read_value = storage.get(id);
    TEST_ASSERT_TRUE(read_value);
    TEST_ASSERT_EQUAL(persisted_value + current_value, *read_value);
    TEST_ASSERT_EQUAL(persisted_value + current_value, counter.get());

    const unsigned new_value = 10 * current_value;

    clock.value = new_value * resolution;
    TEST_ASSERT_EQUAL(persisted_value + current_value, counter.get());

    counter.update(state_set);
    TEST_ASSERT_FALSE(storage.get(id));
    TEST_ASSERT_EQUAL(new_value - current_value - persisted_value, counter.get());
}

TEST_CASE("State counter: persist state on task run",
          "[ocs_diagnostic], [state_counter]") {
    const char* id = "foo";

    const core::microseconds_t resolution = core::Second;

    const StateCounter::State required_state = 1;
    const StateCounter::State new_state = 2;

    const unsigned current_value = 25;

    test::TestClock clock;
    clock.value = current_value * resolution;

    test::TestCounterStorage storage;
    TEST_ASSERT_FALSE(storage.get(id));

    StateCounter counter(storage, clock, id, resolution, required_state);

    // Counter is inactive.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, counter.run());
    TEST_ASSERT_FALSE(storage.get(id));

    // Counter is stil inactive.
    counter.update(new_state);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, counter.run());
    TEST_ASSERT_FALSE(storage.get(id));

    // Counter has become active.
    counter.update(required_state);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, counter.run());

    const auto read_value = storage.get(id);
    TEST_ASSERT_TRUE(read_value);
    TEST_ASSERT_EQUAL(current_value, *read_value);
}

TEST_CASE("State counter: save counter when reboot is happened",
          "[ocs_diagnostic], [state_counter]") {
    const char* id = "foo";

    const core::microseconds_t resolution = core::Second;

    const StateCounter::State required_state = 1;
    const StateCounter::State new_state = 2;

    const unsigned current_value = 25;

    test::TestClock clock;
    clock.value = current_value * resolution;

    test::TestCounterStorage storage;
    TEST_ASSERT_FALSE(storage.get(id));

    StateCounter counter(storage, clock, id, resolution, required_state);

    TEST_ASSERT_FALSE(storage.get(id));
    TEST_ASSERT_EQUAL(0, counter.get());

    // There was no previous state, ensure no data.
    counter.update(new_state);
    TEST_ASSERT_FALSE(storage.get(id));
    TEST_ASSERT_EQUAL(0, counter.get());

    // There was no previous state, ensure no data.
    counter.handle_reboot();
    TEST_ASSERT_FALSE(storage.get(id));
    TEST_ASSERT_EQUAL(0, counter.get());

    // Required state is set.
    counter.update(required_state);
    TEST_ASSERT_FALSE(storage.get(id));
    TEST_ASSERT_EQUAL(current_value, counter.get());

    counter.handle_reboot();
    TEST_ASSERT_TRUE(storage.get(id));
    TEST_ASSERT_EQUAL(current_value, counter.get());
}

} // namespace diagnostic
} // namespace ocs
