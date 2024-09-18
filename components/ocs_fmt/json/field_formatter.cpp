/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_fmt/json/field_formatter.h"

namespace ocs {
namespace fmt {
namespace json {

FieldFormatter::FieldFormatter(const char* key, FieldFormatter::Type type)
    : key_(key)
    , type_(type) {
}

status::StatusCode FieldFormatter::format(cJSON* json) {
    auto field = make_field_();
    if (!field) {
        return status::StatusCode::NoMem;
    }

    for (auto f : formatters_) {
        const auto code = f->format(field.get());
        if (code != status::StatusCode::OK) {
            return code;
        }
    }

    if (!cJSON_AddItemToObjectCS(json, key_.c_str(), field.get())) {
        return status::StatusCode::NoMem;
    }
    field.release();

    return status::StatusCode::OK;
}

void FieldFormatter::add(IFormatter& formatter) {
    formatters_.emplace_back(&formatter);
}

CjsonUniqueBuilder::Ptr FieldFormatter::make_field_() const {
    switch (type_) {
    case Type::Object:
        return CjsonUniqueBuilder::make_object();

    case Type::Array:
        return CjsonUniqueBuilder::make_array();

    default:
        break;
    }

    // Should not happen.
    return CjsonUniqueBuilder::make_nullptr();
}

} // namespace json
} // namespace fmt
} // namespace ocs
