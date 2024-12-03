/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_fmt/json/time_formatter.h"
#include "ocs_algo/time_ops.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace fmt {
namespace json {

status::StatusCode TimeFormatter::format(cJSON* json) {
    CjsonObjectFormatter formatter(json);

    const auto timestamp = algo::TimeOps::get_time();
    if (!timestamp) {
        return status::StatusCode::Error;
    }

    if (!formatter.add_number_cs("timestamp", *timestamp)) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace json
} // namespace fmt
} // namespace ocs
