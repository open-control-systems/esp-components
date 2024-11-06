/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_algo/string_ops.h"

namespace ocs {
namespace algo {

TEST_CASE("String ops: split: empty value", "[ocs_algo], [string_ops]") {
    TEST_ASSERT_EQUAL(0, StringOps::split(':', "").size());
}

TEST_CASE("String ops: split: single character: equal to delimiter",
          "[ocs_algo], [string_ops]") {
    TEST_ASSERT_EQUAL(0, StringOps::split('?', "?").size());
}

TEST_CASE("String ops: split: single character: without delimiter",
          "[ocs_algo], [string_ops]") {
    TEST_ASSERT_EQUAL(0, StringOps::split(':', "a").size());
}

TEST_CASE("String ops: split: single character: with delimiter",
          "[ocs_algo], [string_ops]") {
    const auto values = StringOps::split(':', "a:");
    TEST_ASSERT_EQUAL(1, values.size());
    TEST_ASSERT_TRUE("a" == values[0]);
}

TEST_CASE("String ops: split: invalid delimiter", "[ocs_algo], [string_ops]") {
    TEST_ASSERT_EQUAL(0, StringOps::split('?', "AA:BB:CC:DD:EE:FF").size());
}

TEST_CASE("String ops: split: multiple delimiters", "[ocs_algo], [string_ops]") {
    const auto values = StringOps::split(':', ":::A:BB::CCC::D");
    TEST_ASSERT_EQUAL(4, values.size());

    TEST_ASSERT_TRUE(values[0] == "A");
    TEST_ASSERT_TRUE(values[1] == "BB");
    TEST_ASSERT_TRUE(values[2] == "CCC");
    TEST_ASSERT_TRUE(values[3] == "D");
}

TEST_CASE("String ops: split: valid string", "[ocs_algo], [string_ops]") {
    const auto values = StringOps::split(':', "AA:BB:CC:DD:EE:FF");
    TEST_ASSERT_EQUAL(6, values.size());

    TEST_ASSERT_TRUE(values[0] == "AA");
    TEST_ASSERT_TRUE(values[1] == "BB");
    TEST_ASSERT_TRUE(values[2] == "CC");
    TEST_ASSERT_TRUE(values[3] == "DD");
    TEST_ASSERT_TRUE(values[4] == "EE");
    TEST_ASSERT_TRUE(values[5] == "FF");
}

} // namespace algo
} // namespace ocs
