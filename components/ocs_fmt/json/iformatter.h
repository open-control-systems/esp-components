/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "cJSON.h"

#include "ocs_status/code.h"

namespace ocs {
namespace fmt {
namespace json {

class IFormatter {
public:
    //! Destroy.
    virtual ~IFormatter() = default;

    //! Format JSON.
    virtual status::StatusCode format(cJSON* json) = 0;
};

} // namespace json
} // namespace fmt
} // namespace ocs
