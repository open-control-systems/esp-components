/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_diagnostic/persistent_counter.h"
#include "ocs_test/test_counter.h"
#include "ocs_test/test_storage.h"

namespace ocs {
namespace diagnostic {

namespace {

using TestStorage = test::TestStorage<diagnostic::ICounter::Value>;

} // namespace

TEST_CASE("Persistent counter: without initial value",
          "[ocs_diagnostic], [persistent_counter]") {
    const unsigned counter_value = 42;

    TestStorage storage;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    PersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(counter_value, persistent_counter.get());
}

TEST_CASE("Persistent counter: with initial value",
          "[ocs_diagnostic], [persistent_counter]") {
    const unsigned counter_value = 42;
    const unsigned persisted_value = 43;

    TestStorage storage;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    storage.set(counter.id(), persisted_value);

    PersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(counter_value + persisted_value, persistent_counter.get());
}

TEST_CASE("Persistent counter: handle reboot: without initial value",
          "[ocs_diagnostic], [persistent_counter]") {
    const unsigned counter_value = 42;

    TestStorage storage;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    PersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(counter_value, persistent_counter.get());

    TEST_ASSERT_FALSE(storage.get(counter.id()));
    persistent_counter.handle_reboot();

    const auto read_value = storage.get(counter.id());
    TEST_ASSERT_TRUE(read_value);
    TEST_ASSERT_EQUAL(counter_value, *read_value);
}

TEST_CASE("Persistent counter: handle reboot: with initial value",
          "[ocs_diagnostic], [persistent_counter]") {
    const unsigned counter_value = 42;
    const unsigned persisted_value = 43;

    TestStorage storage;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    storage.set(counter.id(), persisted_value);

    PersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(counter_value + persisted_value, persistent_counter.get());

    auto read_value = storage.get(counter.id());
    TEST_ASSERT_TRUE(read_value);
    TEST_ASSERT_EQUAL(persisted_value, *read_value);

    persistent_counter.handle_reboot();

    read_value = storage.get(counter.id());
    TEST_ASSERT_TRUE(read_value);
    TEST_ASSERT_EQUAL(counter_value + persisted_value, *read_value);
}

TEST_CASE("Persistent counter: invalidate value: with previous value",
          "[ocs_diagnostic], [persistent_counter]") {
    const unsigned current_value = 42;
    const unsigned persisted_value = 43;

    const char* id = "foo";

    TestStorage storage;

    test::TestCounter counter(id);
    counter.value = current_value;

    storage.set(counter.id(), persisted_value);

    PersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(current_value + persisted_value, persistent_counter.get());

    auto read_value = storage.get(counter.id());
    TEST_ASSERT_TRUE(read_value);
    TEST_ASSERT_EQUAL(persisted_value, *read_value);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, persistent_counter.invalidate());

    TEST_ASSERT_FALSE(storage.get(id));
    TEST_ASSERT_EQUAL(current_value, persistent_counter.get());
}

TEST_CASE("Persistent counter: invalidate value: without previous value",
          "[ocs_diagnostic], [persistent_counter]") {
    const unsigned current_value = 42;
    const char* id = "foo";

    TestStorage storage;

    test::TestCounter counter(id);
    counter.value = current_value;

    PersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(current_value, persistent_counter.get());

    TEST_ASSERT_FALSE(storage.get(counter.id()));
    TEST_ASSERT_EQUAL(status::StatusCode::NoData, persistent_counter.invalidate());

    TEST_ASSERT_FALSE(storage.get(id));
    TEST_ASSERT_EQUAL(current_value, persistent_counter.get());
}

TEST_CASE("Persistent counter: save value: without previous value",
          "[ocs_diagnostic], [persistent_counter]") {
    const unsigned current_value = 42;
    const char* id = "foo";

    TestStorage storage;

    test::TestCounter counter(id);
    counter.value = current_value;

    PersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(current_value, persistent_counter.get());

    TEST_ASSERT_FALSE(storage.get(counter.id()));

    const unsigned new_value = current_value * 3;
    counter.value = new_value;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, persistent_counter.save());

    const auto read_value = storage.get(id);
    TEST_ASSERT_TRUE(read_value);
    TEST_ASSERT_EQUAL(new_value, *read_value);

    TEST_ASSERT_EQUAL(new_value, persistent_counter.get());
}

TEST_CASE("Persistent counter: save value: with previous value",
          "[ocs_diagnostic], [persistent_counter]") {
    const unsigned current_value = 42;
    const unsigned persisted_value = 43;

    const char* id = "foo";

    TestStorage storage;

    test::TestCounter counter(id);
    counter.value = current_value;

    storage.set(counter.id(), persisted_value);

    PersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(current_value + persisted_value, persistent_counter.get());

    TEST_ASSERT_TRUE(storage.get(counter.id()));

    const unsigned new_value = current_value * 3;
    counter.value = new_value;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, persistent_counter.save());

    const auto read_value = storage.get(id);
    TEST_ASSERT_TRUE(read_value);
    TEST_ASSERT_EQUAL(persisted_value + new_value, *read_value);

    TEST_ASSERT_EQUAL(new_value + persisted_value, persistent_counter.get());
}

TEST_CASE("Persistent counter: save value on task run",
          "[ocs_diagnostic], [persistent_counter]") {
    const unsigned counter_value = 42;

    TestStorage storage;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    PersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(counter_value, persistent_counter.get());

    TEST_ASSERT_FALSE(storage.get(counter.id()));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, persistent_counter.run());

    const auto read_value = storage.get(counter.id());
    TEST_ASSERT_TRUE(read_value);
    TEST_ASSERT_EQUAL(counter_value, *read_value);
}

} // namespace diagnostic
} // namespace ocs
