/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_algo/crc_ops.h"

namespace ocs {
namespace algo {

uint8_t CrcOps::crc8(const uint8_t* buf,
                     unsigned size,
                     uint8_t initial,
                     uint8_t polynomial,
                     CrcOps::BitOrder order) {
    // Initialize shift register.
    uint8_t crc = initial;

    for (unsigned n = 0; n < size; ++n) {
        // XOR data byte with current CRC.
        crc ^= buf[n];

        for (uint8_t bit = 0; bit < 8; ++bit) {
            const bool bit_set =
                (order == CrcOps::BitOrder::LSB) ? (crc & 0x01) : (crc & 0x80);

            if (bit_set) {
                crc = (order == CrcOps::BitOrder::LSB) ? (crc >> 1) ^ polynomial
                                                       : ((crc << 1) ^ polynomial) & 0xFF;
            } else {
                crc = (order == CrcOps::BitOrder::LSB) ? (crc >> 1) : (crc << 1) & 0xFF;
            }
        }
    }

    return crc;
}

} // namespace algo
} // namespace ocs
