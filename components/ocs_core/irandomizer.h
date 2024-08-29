/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstddef>
#include <cstdint>

#pragma once

namespace ocs {
namespace core {

class IRandomizer {
public:
    //! Destroy
    virtual ~IRandomizer() = default;

    //! Generate a random value in the range [from; to].
    virtual uint32_t random(uint32_t from, uint32_t to) = 0;
};

} // namespace core
} // namespace ocs
