/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/time.h"

namespace ocs {
namespace algo {

struct TimeOps {
    //! Return true if @p interval is passed since @p start time.
    template <typename T> static bool after(T start, T now, T interval) {
        const T dest = start + interval;

        if (now < start) {
            if (dest > start) {
                return true;
            }

            if (dest <= now) {
                return true;
            }

            return false;
        }

        if (dest > now) {
            return false;
        }

        if (dest < start) {
            return false;
        }

        return true;
    }
};

} // namespace algo
} // namespace ocs
