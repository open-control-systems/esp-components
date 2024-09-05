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
namespace core {

struct BitOps {
    //! Create a mask with the n-th bit set.
    static unsigned mask(uint8_t pos);

    //! Create a mask with the n-th bit unset.
    static unsigned umask(uint8_t pos);

    //! Return the n-th bit of the value.
    static uint8_t nth(unsigned value, uint8_t pos);
};

} // namespace core
} // namespace ocs
