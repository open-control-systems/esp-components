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
namespace diagnostic {

class ICounter {
public:
    using Value = uint64_t;

    //! Destroy.
    virtual ~ICounter() = default;

    //! Return a unique identifier to distinguish one counter from another.
    virtual const char* id() const = 0;

    //! Return the current counter value.
    virtual Value get() = 0;
};

} // namespace diagnostic
} // namespace ocs
