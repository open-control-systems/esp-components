/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_http/uri_ops.h"

namespace ocs {
namespace http {

TEST_CASE("URI ops: parse path: null input", "[ocs_http], [uri_ops]") {
    TEST_ASSERT_TRUE(UriOps::parse_path(nullptr) == std::string_view());
}

TEST_CASE("URI ops: parse path: empty", "[ocs_http], [uri_ops]") {
    TEST_ASSERT_TRUE(UriOps::parse_path("") == std::string_view());
}

TEST_CASE("URI ops: parse path: without query", "[ocs_http], [uri_ops]") {
    TEST_ASSERT_TRUE(UriOps::parse_path("/foo/bar/baz") == "/foo/bar/baz");
}

TEST_CASE("URI ops: parse path: with query", "[ocs_http], [uri_ops]") {
    TEST_ASSERT_TRUE(UriOps::parse_path("/foo/bar/baz?key=val") == "/foo/bar/baz");
}

TEST_CASE("URI ops: parse query: null input", "[ocs_http], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query(nullptr).size());
}

TEST_CASE("URI ops: parse query: empty inut", "[ocs_http], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("").size());
}

TEST_CASE("URI ops: parse query: no query", "[ocs_http], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar").size());
}

TEST_CASE("URI ops: parse query: key missed", "[ocs_http], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?").size());
}

TEST_CASE("URI ops: parse query: wrong delimiter", "[ocs_http], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?key:").size());
}

TEST_CASE("URI ops: parse query: key-value missed", "[ocs_http], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?=").size());
}

TEST_CASE("URI ops: parse query: value missed", "[ocs_http], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?key=").size());
}

TEST_CASE("URI ops: parse query: single pair", "[ocs_http], [uri_ops]") {
    const auto values = UriOps::parse_query("/foo/bar?key=value");
    TEST_ASSERT_EQUAL(1, values.size());

    const auto it = values.find("key");
    TEST_ASSERT_TRUE(it != values.end());
    TEST_ASSERT_TRUE(it->first == "key");
    TEST_ASSERT_TRUE(it->second == "value");
}

TEST_CASE("URI ops: parse query: multile pairs", "[ocs_http], [uri_ops]") {
    const auto values = UriOps::parse_query("/foo/bar?key1=value1&key2=value2");
    TEST_ASSERT_EQUAL(2, values.size());

    auto it = values.find("key1");
    TEST_ASSERT_TRUE(it != values.end());
    TEST_ASSERT_TRUE(it->first == "key1");
    TEST_ASSERT_TRUE(it->second == "value1");

    it = values.find("key2");
    TEST_ASSERT_TRUE(it != values.end());
    TEST_ASSERT_TRUE(it->first == "key2");
    TEST_ASSERT_TRUE(it->second == "value2");
}

} // namespace http
} // namespace ocs
