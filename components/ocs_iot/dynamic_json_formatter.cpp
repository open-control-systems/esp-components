/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_iot/dynamic_json_formatter.h"

namespace ocs {
namespace iot {

DynamicJsonFormatter::DynamicJsonFormatter(unsigned size)
    : size_(size) {
    configASSERT(size_);

    buf_.reset(new (std::nothrow) char[size]);
    configASSERT(buf_);
}

void DynamicJsonFormatter::format(cJSON* json) {
    clear_();
    configASSERT(cJSON_PrintPreallocated(json, buf_.get(), size_, false));
}

const char* DynamicJsonFormatter::c_str() const {
    return buf_.get();
}

void DynamicJsonFormatter::clear_() {
    memset(buf_.get(), 0, size_);
}

} // namespace iot
} // namespace ocs
