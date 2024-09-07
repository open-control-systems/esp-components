/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_net/uri_ops.h"

namespace ocs {
namespace net {

TEST_CASE("URI ops: parse path: null input", "[ocs_net], [uri_ops]") {
    TEST_ASSERT_TRUE(std::string_view() == UriOps::parse_path(nullptr));
}

TEST_CASE("URI ops: parse path: empty", "[ocs_net], [uri_ops]") {
    TEST_ASSERT_TRUE(std::string_view() == UriOps::parse_path(""));
}

TEST_CASE("URI ops: parse path: without query", "[ocs_net], [uri_ops]") {
    TEST_ASSERT_TRUE(std::string_view("/foo/bar/baz")
                     == UriOps::parse_path("/foo/bar/baz"));
}

TEST_CASE("URI ops: parse path: with query", "[ocs_net], [uri_ops]") {
    TEST_ASSERT_TRUE(std::string_view("/foo/bar/baz")
                     == UriOps::parse_path("/foo/bar/baz?key=val"));
}

TEST_CASE("URI ops: parse query: null input", "[ocs_net], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query(nullptr).size());
}

TEST_CASE("URI ops: parse query: empty inut", "[ocs_net], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("").size());
}

TEST_CASE("URI ops: parse query: no query", "[ocs_net], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar").size());
}

TEST_CASE("URI ops: parse query: key missed", "[ocs_net], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?").size());
}

TEST_CASE("URI ops: parse query: wrong delimiter", "[ocs_net], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?key:").size());
}

TEST_CASE("URI ops: parse query: key-value missed", "[ocs_net], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?=").size());
}

TEST_CASE("URI ops: parse query: value missed", "[ocs_net], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?key=").size());
}

TEST_CASE("URI ops: parse query: single pair", "[ocs_net], [uri_ops]") {
    const auto values = UriOps::parse_query("/foo/bar?key=value");
    TEST_ASSERT_EQUAL(1, values.size());

    const auto it = values.find("key");
    TEST_ASSERT_TRUE(it != values.end());
    TEST_ASSERT_TRUE(std::string_view("key") == it->first);
    TEST_ASSERT_TRUE(std::string_view("value") == it->second);
}

TEST_CASE("URI ops: parse query: multile pairs", "[ocs_net], [uri_ops]") {
    const auto values = UriOps::parse_query("/foo/bar?key1=value1&key2=value2");
    TEST_ASSERT_EQUAL(2, values.size());

    auto it = values.find("key1");
    TEST_ASSERT_TRUE(it != values.end());
    TEST_ASSERT_TRUE(std::string_view("key1") == it->first);
    TEST_ASSERT_TRUE(std::string_view("value1") == it->second);

    it = values.find("key2");
    TEST_ASSERT_TRUE(it != values.end());
    TEST_ASSERT_TRUE(std::string_view("key2") == it->first);
    TEST_ASSERT_TRUE(std::string_view("value2") == it->second);
}

} // namespace net
} // namespace ocs
