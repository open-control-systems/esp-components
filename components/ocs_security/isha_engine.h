/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>

#include "ocs_status/code.h"

namespace ocs {
namespace security {

class IShaEngine {
public:
    //! Known SHA algorithms.
    enum class Algorithm {
        SHA1,
        SHA256,
        SHA512,
    };

    //! Destroy.
    virtual ~IShaEngine() = default;

    //! Return algorithm used for SHA calculation.
    virtual Algorithm get_algorithm() const = 0;

    //! Generate SHA to @p buf from @p size bytes from @p src.
    //!
    //! @remarks
    //!  - @p src should be at least @p size bytes long.
    //!  - @p buf should be large enough to store the result of SHA calculation.
    virtual status::StatusCode
    generate(uint8_t* buf, const uint8_t* src, unsigned size) = 0;
};

} // namespace security
} // namespace ocs
