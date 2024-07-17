/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_iot/string_json_formatter.h"

namespace ocs {
namespace iot {

class VersionJSONFormatter : public StringJSONFormatter, public core::NonCopyable<> {
public:
    //! Add ESP-IDF version on initialization.
    VersionJSONFormatter();
};

} // namespace iot
} // namespace ocs
