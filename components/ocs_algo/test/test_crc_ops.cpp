/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "unity.h"

#include "ocs_algo/bit_ops.h"
#include "ocs_algo/crc_ops.h"

namespace ocs {
namespace algo {

TEST_CASE("CRC Ops: crc8: MSB", "[ocs_algo], [crc_ops]") {
    const uint16_t data = 0xBEEF;

    uint8_t buf[2];
    memset(buf, 0, sizeof(buf));

    algo::BitOps::unpack_u8(data, buf[0], buf[1]);

    TEST_ASSERT_EQUAL(0x92,
                      CrcOps::crc8(buf, sizeof(buf), 0xFF, 0x31, CrcOps::BitOrder::MSB));
}

TEST_CASE("CRC Ops: crc8: LSB", "[ocs_algo], [crc_ops]") {
    const uint8_t buf[] {
        0xAB, 0x1, 0x4B, 0x46, 0x7F, 0xFF, 0x5, 0x10,
    };

    TEST_ASSERT_EQUAL(0x92,
                      CrcOps::crc8(buf, sizeof(buf), 0x00, 0x8C, CrcOps::BitOrder::LSB));
}

} // namespace algo
} // namespace ocs
