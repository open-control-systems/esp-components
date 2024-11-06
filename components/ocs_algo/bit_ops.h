/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>

namespace ocs {
namespace algo {

struct BitOps {
    //! Create a mask with the n-th bit set.
    static unsigned mask(uint8_t pos);

    //! Create a mask with the n-th bit unset.
    static unsigned umask(uint8_t pos);

    //! Return the n-th bit of the value.
    static uint8_t nth(unsigned value, uint8_t pos);

    //! Return 16-bit value from two 8-bit values.
    static uint16_t pack_u8(uint8_t hi, uint8_t lo);

    //! Return 32-bit value from two 16-bit values.
    static uint32_t pack_u16(uint16_t hi, uint16_t lo);

    //! Unpack 16-bit value into corresponding 8-bit values.
    static void unpack_u8(uint16_t value, uint8_t& hi, uint8_t& lo);
};

} // namespace algo
} // namespace ocs
