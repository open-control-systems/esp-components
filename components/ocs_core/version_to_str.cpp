/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstdio>

#include "ocs_core/version_to_str.h"

namespace ocs {
namespace core {

version_to_str::version_to_str(Version version) {
    memset(buf_, 0, sizeof(buf_));

    if (snprintf(buf_, sizeof(buf_), "%u.%u.%u", static_cast<unsigned>(version.major),
                 static_cast<unsigned>(version.minor),
                 static_cast<unsigned>(version.patch))
        < 0) {
        strcpy(buf_, "<none>");
    }
}

const char* version_to_str::c_str() const {
    return buf_;
}

} // namespace core
} // namespace ocs
