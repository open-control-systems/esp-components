/*
 * Copyright (c) 2024, Open Control System authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cassert>

#include "ocs_iot/cjson_array_formatter.h"
#include "ocs_iot/cjson_builder.h"

namespace ocs {
namespace iot {

cJSONArrayFormatter::cJSONArrayFormatter(cJSON* json)
    : json_(json) {
    assert(cJSON_IsArray(json_));
}

bool cJSONArrayFormatter::append_uint16(uint16_t value) {
    auto item = cJSONUniqueBuilder::make_json_number(value);
    if (!item) {
        return false;
    }

    if (!cJSON_AddItemToArray(json_, item.get())) {
        return false;
    }

    item.release();
    return true;
}

} // namespace iot
} // namespace ocs