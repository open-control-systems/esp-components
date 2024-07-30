/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_iot/string_json_formatter.h"
#include "ocs_iot/cjson_object_formatter.h"

namespace ocs {
namespace iot {

void StringJsonFormatter::format(cJSON* json) {
    CjsonObjectFormatter formatter(json);

    for (const auto& [key, val] : values_) {
        formatter.add_string_ref_cs(key.c_str(), val.c_str());
    }
}

void StringJsonFormatter::add(const char* key, const char* val) {
    values_[key] = val;
}

} // namespace iot
} // namespace ocs
