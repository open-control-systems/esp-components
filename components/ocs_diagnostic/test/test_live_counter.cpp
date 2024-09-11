/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_diagnostic/live_counter.h"
#include "ocs_test/test_counter.h"
#include "ocs_test/test_storage.h"

namespace ocs {
namespace diagnostic {

namespace {

using TestStorage = test::TestStorage<diagnostic::ICounter::Value>;

} // namespace

TEST_CASE("Live counter: erase on initialization", "[ocs_diagnostic], [live_counter]") {
    const unsigned counter_value = 7;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    const unsigned persisted_value = 42;

    TestStorage storage;
    storage.set(counter.id(), persisted_value);

    TEST_ASSERT_TRUE(persisted_value != counter_value);
    TEST_ASSERT_TRUE(storage.get(counter.id()));

    LiveCounter live_counter(storage, counter);

    TEST_ASSERT_FALSE(storage.get(counter.id()));
    TEST_ASSERT_EQUAL(persisted_value + counter_value, live_counter.get());
}

} // namespace diagnostic
} // namespace ocs
