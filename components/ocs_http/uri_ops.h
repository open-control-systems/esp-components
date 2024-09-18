/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string_view>
#include <unordered_map>

namespace ocs {
namespace http {

struct UriOps {
    using Path = std::string_view;
    using Values = std::unordered_map<std::string_view, std::string_view>;

    //! Extract path from the @p uri.
    //!
    //! @remarks
    //!  @p uri should be valid while the returned path is being used.
    static Path parse_path(const char* uri);

    //! Extract key-value query fields from the @p uri.
    //!
    //! @remarks
    //!  @p uri should be valid while the returned values are being used.
    static Values parse_query(const char* uri);
};

} // namespace http
} // namespace ocs
