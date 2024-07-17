/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstdio>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/version.h"

namespace ocs {
namespace core {

Version::Version(uint16_t major, uint16_t minor, uint16_t patch)
    : major(major)
    , minor(minor)
    , patch(patch) {
}

bool Version::operator==(const Version& version) const {
    return memcmp(this, &version, sizeof(Version)) == 0;
}

bool Version::operator!=(const Version& version) const {
    return !(*this == version);
}

bool Version::operator<(const Version& version) const {
    if (major < version.major) {
        return true;
    }

    if (minor < version.minor) {
        return true;
    }

    return patch < version.patch;
}

bool Version::parse(const char* str) {
    configASSERT(str);

    uint32_t parsed_major = 0;
    uint32_t parsed_minor = 0;
    uint32_t parsed_patch = 0;

    if (sscanf(str, "%lu.%lu.%lu", &parsed_major, &parsed_minor, &parsed_patch) != 3
        && sscanf(str, "%lu.%lu", &parsed_major, &parsed_minor) != 2
        && sscanf(str, "%lu", &parsed_major) != 1) {
        return false;
    }

    if (parsed_major > UINT16_MAX || parsed_minor > UINT16_MAX
        || parsed_patch > UINT16_MAX) {
        return false;
    }

    major = parsed_major;
    minor = parsed_minor;
    patch = parsed_patch;

    return true;
}

} // namespace core
} // namespace ocs
