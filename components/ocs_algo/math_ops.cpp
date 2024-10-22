/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include "ocs_algo/math_ops.h"

namespace ocs {
namespace algo {

double MathOps::round_floor(double value, unsigned decimal_places) {
    const auto multiplier = std::pow(10.0, decimal_places);
    const auto ret = std::floor(value * multiplier) / multiplier;
    return ret;
}

} // namespace algo
} // namespace ocs
