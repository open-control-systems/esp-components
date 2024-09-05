/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_core/bit_ops.h"

namespace ocs {
namespace core {

unsigned BitOps::mask(uint8_t pos) {
    return (1UL << pos);
}

unsigned BitOps::umask(uint8_t pos) {
    return ~(mask(pos));
}

uint8_t BitOps::nth(unsigned value, uint8_t pos) {
    return mask(pos) & value ? 1 : 0;
}

} // namespace core
} // namespace ocs
