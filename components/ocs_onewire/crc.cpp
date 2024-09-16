/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_onewire/crc.h"

namespace ocs {
namespace onewire {

uint8_t calculate_crc(const uint8_t* buf, unsigned size) {
    // Initialize shift register to 0.
    uint8_t crc = 0;

    for (unsigned n = 0; n < size; ++n) {
        // XOR data byte with current CRC value.
        crc ^= buf[n];

        for (uint8_t bit = 0; bit < 8; ++bit) {
            if (crc & 0x01) {
                // Polynomial 0x8C is used for DS18B20.
                crc = (crc >> 1) ^ 0x8C;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

} // namespace onewire
} // namespace ocs
