/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_algo/bit_ops.h"

namespace ocs {
namespace algo {

unsigned BitOps::mask(uint8_t pos) {
    return (1UL << pos);
}

unsigned BitOps::umask(uint8_t pos) {
    return ~(mask(pos));
}

uint8_t BitOps::nth(unsigned value, uint8_t pos) {
    return mask(pos) & value ? 1 : 0;
}

uint16_t BitOps::pack_u8(uint8_t hi, uint8_t lo) {
    return static_cast<uint16_t>(hi) << 8 | lo;
}

uint32_t BitOps::pack_u16(uint16_t hi, uint16_t lo) {
    return static_cast<uint32_t>(hi) << 16 | lo;
}

void BitOps::unpack_u8(uint16_t value, uint8_t& hi, uint8_t& lo) {
    hi = (value & 0xFF00) >> 8;
    lo = value & 0x00FF;
}

} // namespace algo
} // namespace ocs
