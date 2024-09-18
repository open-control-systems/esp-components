/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_fmt/json/string_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace fmt {
namespace json {

status::StatusCode StringFormatter::format(cJSON* json) {
    CjsonObjectFormatter formatter(json);

    for (const auto& [key, val] : values_) {
        if (!formatter.add_string_ref_cs(key.c_str(), val.c_str())) {
            return status::StatusCode::NoMem;
        }
    }

    return status::StatusCode::OK;
}

void StringFormatter::add(const char* key, const char* val) {
    values_[key] = val;
}

} // namespace json
} // namespace fmt
} // namespace ocs
