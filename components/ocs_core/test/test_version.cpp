/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstdint>
#include <string>
#include <vector>

#include "unity.h"

#include "ocs_core/version.h"
#include "ocs_core/version_to_str.h"

namespace ocs {
namespace core {

TEST_CASE("Compare versions: equal", "[ocs_core], [version]") {
    TEST_ASSERT_TRUE(Version(0, 0, 0) == Version(0, 0, 0));
    TEST_ASSERT_TRUE(Version(1, 0, 0) == Version(1, 0, 0));
    TEST_ASSERT_TRUE(Version(1, 1, 0) == Version(1, 1, 0));
    TEST_ASSERT_TRUE(Version(1, 0, 1) == Version(1, 0, 1));
    TEST_ASSERT_TRUE(Version(1, 1, 1) == Version(1, 1, 1));
}

TEST_CASE("Compare versions: non-equal", "[ocs_core], [version]") {
    TEST_ASSERT_TRUE(Version(1, 1, 1) != Version(0, 1, 1));
    TEST_ASSERT_TRUE(Version(1, 1, 1) != Version(1, 0, 1));
    TEST_ASSERT_TRUE(Version(1, 1, 1) != Version(1, 0, 0));
}

TEST_CASE("Compare versions: less", "[ocs_core], [version]") {
    TEST_ASSERT_FALSE(Version(0, 0, 0) < Version(0, 0, 0));

    TEST_ASSERT_TRUE(Version(0, 0, 1) < Version(0, 1, 0));
    TEST_ASSERT_TRUE(Version(0, 1, 0) < Version(1, 0, 0));
    TEST_ASSERT_TRUE(Version(1, 0, 0) < Version(2, 0, 0));
}

TEST_CASE("Parse version: empty", "[ocs_core], [version]") {
    Version version;
    TEST_ASSERT_FALSE(version.parse(""));
    TEST_ASSERT_EQUAL(0, version.major);
    TEST_ASSERT_EQUAL(0, version.minor);
    TEST_ASSERT_EQUAL(0, version.patch);
}

TEST_CASE("Parse version: no-digit input", "[ocs_core], [version]") {
    Version version;
    TEST_ASSERT_FALSE(version.parse("."));
    TEST_ASSERT_FALSE(version.parse(".."));
    TEST_ASSERT_FALSE(version.parse("..."));
}

TEST_CASE("Parse version: major-only input", "[ocs_core], [version]") {
    Version version;
    TEST_ASSERT_TRUE(version.parse("9"));
    TEST_ASSERT_EQUAL(9, version.major);
    TEST_ASSERT_EQUAL(0, version.minor);
    TEST_ASSERT_EQUAL(0, version.patch);
}

TEST_CASE("Parse version: major-minor input", "[ocs_core], [version]") {
    Version version;
    TEST_ASSERT_TRUE(version.parse("9.1"));
    TEST_ASSERT_EQUAL(9, version.major);
    TEST_ASSERT_EQUAL(1, version.minor);
    TEST_ASSERT_EQUAL(0, version.patch);
}

TEST_CASE("Parse version: random numbers", "[ocs_core], [version]") {
    Version version;
    TEST_ASSERT_TRUE(version.parse("0.0.7"));
    TEST_ASSERT_EQUAL(0, version.major);
    TEST_ASSERT_EQUAL(0, version.minor);
    TEST_ASSERT_EQUAL(7, version.patch);
}

TEST_CASE("Parse version: max numbers", "[ocs_core], [version]") {
    Version version;
    TEST_ASSERT_TRUE(version.parse("65535.65535.65535"));
    TEST_ASSERT_EQUAL(UINT16_MAX, version.major);
    TEST_ASSERT_EQUAL(UINT16_MAX, version.minor);
    TEST_ASSERT_EQUAL(UINT16_MAX, version.patch);
}

TEST_CASE("Parse version: overflow", "[ocs_core], [version]") {
    const std::vector<std::string> versions {
        "65536.65535.65535", "65535.65536.65535", "65535.65535.65536",
        "65536.65536.65535", "65535.65536.65536", "65536.65535.65536",
        "65536.65536.65536",
    };

    for (const auto& str : versions) {
        Version version;
        TEST_ASSERT_FALSE(version.parse(str.c_str()));
        TEST_ASSERT_EQUAL(0, version.major);
        TEST_ASSERT_EQUAL(0, version.minor);
        TEST_ASSERT_EQUAL(0, version.patch);
    }
}

TEST_CASE("Parse version: omit non-core characters", "[ocs_core], [version]") {
    Version version;
    TEST_ASSERT_TRUE(version.parse("9.1.1_<rc|beta|alpha|etc|_123>"));
    TEST_ASSERT_EQUAL(9, version.major);
    TEST_ASSERT_EQUAL(1, version.minor);
    TEST_ASSERT_EQUAL(1, version.patch);
}

} // namespace core
} // namespace ocs
