/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_fmt/json/dynamic_formatter.h"

namespace ocs {
namespace fmt {
namespace json {

DynamicFormatter::DynamicFormatter(unsigned size)
    : size_(size) {
    configASSERT(size_);

    buf_.reset(new (std::nothrow) char[size]);
    configASSERT(buf_);
}

status::StatusCode DynamicFormatter::format(cJSON* json) {
    clear_();

    if (!cJSON_PrintPreallocated(json, buf_.get(), size_, false)) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

const char* DynamicFormatter::c_str() const {
    return buf_.get();
}

void DynamicFormatter::clear_() {
    memset(buf_.get(), 0, size_);
}

} // namespace json
} // namespace fmt
} // namespace ocs
