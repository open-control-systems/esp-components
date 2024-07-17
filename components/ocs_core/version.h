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

struct Version {
    //! Default initialization.
    Version() = default;

    //! Initialize from parameters.
    Version(uint16_t major, uint16_t minor, uint16_t patch);

    bool operator==(const Version&) const;
    bool operator!=(const Version&) const;
    bool operator<(const Version&) const;

    //! Parse version from @p str.
    //!
    //! @notes
    //!  @p str should be in the semver format. See https://semver.org/.
    //!
    //! @return
    //!  True if @p str was successfully parsed;
    bool parse(const char* str);

    uint16_t major { 0 };
    uint16_t minor { 0 };
    uint16_t patch { 0 };
};

} // namespace core
} // namespace ocs
