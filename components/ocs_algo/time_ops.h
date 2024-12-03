/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <ctime>
#include <optional>

#include "ocs_core/time.h"
#include "ocs_status/code.h"

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

    //! Return system time.
    static std::optional<time_t> get_time();

    //! Parse system time from @p str.
    static std::optional<time_t> parse_time(const char* str);

    //! Set the system time from @p str, making sure that the set time is greater
    //! than @p start_point.
    static status::StatusCode set_time(const char* str, time_t start_point);
};

} // namespace algo
} // namespace ocs
