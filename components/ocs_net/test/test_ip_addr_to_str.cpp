/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_net/ip_addr_to_str.h"

namespace ocs {
namespace net {

TEST_CASE("Format IP address: random address", "[ocs_net], [ip_addr_to_str]") {
    const ip_addr_t addr = IPADDR4_INIT_BYTES(192, 168, 1, 137);
    ip_addr_to_str str(addr);
    TEST_ASSERT_EQUAL_STRING("192.168.1.137", str.c_str());
}

TEST_CASE("Format IP address: max address", "[ocs_net], [ip_addr_to_str]") {
    const ip_addr_t addr = IPADDR4_INIT_BYTES(255, 255, 255, 255);
    ip_addr_to_str str(addr);
    TEST_ASSERT_EQUAL_STRING("255.255.255.255", str.c_str());
}

} // namespace net
} // namespace ocs
