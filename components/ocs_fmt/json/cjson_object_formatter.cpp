/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_fmt/json/cjson_builder.h"

namespace ocs {
namespace fmt {
namespace json {

CjsonObjectFormatter::CjsonObjectFormatter(cJSON* json)
    : json_(json) {
}

bool CjsonObjectFormatter::add_string_ref_cs(const char* key, const char* value) {
    auto item = CjsonUniqueBuilder::make_string_ref(value);
    if (!item) {
        return false;
    }

    if (!cJSON_AddItemToObjectCS(json_, key, item.get())) {
        return false;
    }

    item.release();
    return true;
}

bool CjsonObjectFormatter::add_string_cs(const char* key, const char* value) {
    auto item = CjsonUniqueBuilder::make_string(value);
    if (!item) {
        return false;
    }

    if (!cJSON_AddItemToObjectCS(json_, key, item.get())) {
        return false;
    }

    item.release();
    return true;
}

bool CjsonObjectFormatter::add_number_cs(const char* key, double value) {
    auto item = CjsonUniqueBuilder::make_number(value);
    if (!item) {
        return false;
    }

    if (!cJSON_AddItemToObjectCS(json_, key, item.get())) {
        return false;
    }

    item.release();
    return true;
}

bool CjsonObjectFormatter::add_bool_cs(const char* key, bool value) {
    auto item = CjsonUniqueBuilder::make_bool(value);
    if (!item) {
        return false;
    }

    if (!cJSON_AddItemToObjectCS(json_, key, item.get())) {
        return false;
    }

    item.release();
    return true;
}

bool CjsonObjectFormatter::add_true_cs(const char* key) {
    return add_bool_cs(key, true);
}

bool CjsonObjectFormatter::add_false_cs(const char* key) {
    return add_bool_cs(key, false);
}

bool CjsonObjectFormatter::add_null_cs(const char* key) {
    auto item = CjsonUniqueBuilder::make_null();
    if (!item) {
        return false;
    }

    if (!cJSON_AddItemToObjectCS(json_, key, item.get())) {
        return false;
    }

    item.release();
    return true;
}

} // namespace json
} // namespace fmt
} // namespace ocs
