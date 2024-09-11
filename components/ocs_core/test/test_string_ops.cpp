/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_core/string_ops.h"

namespace ocs {
namespace core {

TEST_CASE("String ops: split: empty value", "[ocs_core], [string_ops]") {
    TEST_ASSERT_EQUAL(0, StringOps::split(':', "").size());
}

TEST_CASE("String ops: split: value equal to delimiter", "[ocs_core], [string_ops]") {
    TEST_ASSERT_EQUAL(0, StringOps::split('?', "?").size());
}

TEST_CASE("String ops: split: value equal to delimiter", "[ocs_core], [string_ops]") {
    TEST_ASSERT_EQUAL(0, StringOps::split('?', "?").size());
}

TEST_CASE("String ops: split: invalid delimiter", "[ocs_core], [string_ops]") {
    TEST_ASSERT_EQUAL(0, StringOps::split('?', "AA:BB:CC:DD:EE:FF").size());
}

TEST_CASE("String ops: split: multiple delimiters", "[ocs_core], [string_ops]") {
    const auto values = StringOps::split(':', ":::A:BB::CCC::D");
    TEST_ASSERT_EQUAL(3, values.size());

    TEST_ASSERT_TRUE(StringOps::Value("A") == values[0]);
    TEST_ASSERT_TRUE(StringOps::Value("BB") == values[1]);
    TEST_ASSERT_TRUE(StringOps::Value("CCC") == values[2]);
    TEST_ASSERT_TRUE(StringOps::Value("D") == values[3]);
}

TEST_CASE("String ops: split: valid string", "[ocs_core], [string_ops]") {
    const auto values = StringOps::split(':', "AA:BB:CC:DD:EE:FF");
    TEST_ASSERT_EQUAL(6, values.size());

    TEST_ASSERT_TRUE(StringOps::Value("AA") == values[0]);
    TEST_ASSERT_TRUE(StringOps::Value("BB") == values[1]);
    TEST_ASSERT_TRUE(StringOps::Value("CC") == values[2]);
    TEST_ASSERT_TRUE(StringOps::Value("DD") == values[3]);
    TEST_ASSERT_TRUE(StringOps::Value("EE") == values[4]);
    TEST_ASSERT_TRUE(StringOps::Value("FF") == values[5]);
}

} // namespace core
} // namespace ocs
