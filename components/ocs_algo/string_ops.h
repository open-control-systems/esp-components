/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <string_view>
#include <vector>

namespace ocs {
namespace algo {

struct StringOps {
    using Value = std::string_view;
    using Values = std::vector<Value>;

    //! Split string into substrings.
    static Values split(char delimiter, const Value& value);
};

} // namespace algo
} // namespace ocs
