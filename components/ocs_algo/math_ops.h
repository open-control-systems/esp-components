/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace ocs {
namespace algo {

struct MathOps {
    //! Round @p value down to @p decimal_places.
    static double round_floor(double value, unsigned decimal_places);
};

} // namespace algo
} // namespace ocs
