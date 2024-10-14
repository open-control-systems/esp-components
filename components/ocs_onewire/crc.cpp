/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_onewire/crc.h"
#include "ocs_algo/crc_ops.h"

namespace ocs {
namespace onewire {

uint8_t calculate_crc(const uint8_t* buf, unsigned size) {
    return algo::CrcOps::crc8(buf, size, 0x00, 0x8C, algo::CrcOps::BitOrder::LSB);
}

} // namespace onewire
} // namespace ocs
