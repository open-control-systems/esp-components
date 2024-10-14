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

struct CrcOps {
    //! https://en.wikipedia.org/wiki/Bit_numbering.
    enum class BitOrder {
        //! Least significan bit first.
        LSB,
        //! Most significan bit first.
        MSB,
    };

    //! Calculate CRC-8 checksum.
    //!
    //! @params
    //!
    //! @references
    //!  https://www.boost.org/doc/libs/1_84_0/doc/html/crc/crc_optimal.html
    static uint8_t crc8(const uint8_t* buf,
                        unsigned size,
                        uint8_t initial,
                        uint8_t polynomial,
                        BitOrder order);
};

} // namespace algo
} // namespace ocs
