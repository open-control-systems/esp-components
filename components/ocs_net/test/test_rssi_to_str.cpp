/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_net/rssi_to_str.h"

namespace ocs {
namespace net {

TEST_CASE("Format RSSI string", "[ocs_net], [ip_addr_to_str]") {
    TEST_ASSERT_EQUAL_STRING("excellent", rssi_to_str(-49));

    TEST_ASSERT_EQUAL_STRING("good", rssi_to_str(-50));
    TEST_ASSERT_EQUAL_STRING("good", rssi_to_str(-51));
    TEST_ASSERT_EQUAL_STRING("good", rssi_to_str(-59));

    TEST_ASSERT_EQUAL_STRING("fair", rssi_to_str(-60));
    TEST_ASSERT_EQUAL_STRING("fair", rssi_to_str(-62));
    TEST_ASSERT_EQUAL_STRING("fair", rssi_to_str(-69));

    TEST_ASSERT_EQUAL_STRING("poor", rssi_to_str(-70));
    TEST_ASSERT_EQUAL_STRING("poor", rssi_to_str(-73));
    TEST_ASSERT_EQUAL_STRING("poor", rssi_to_str(-79));

    TEST_ASSERT_EQUAL_STRING("unreliable", rssi_to_str(-80));
    TEST_ASSERT_EQUAL_STRING("unreliable", rssi_to_str(-84));
    TEST_ASSERT_EQUAL_STRING("unreliable", rssi_to_str(-101));
}

} // namespace net
} // namespace ocs
