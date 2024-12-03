/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_fmt/json/fanout_formatter.h"

namespace ocs {
namespace fmt {
namespace json {

status::StatusCode FanoutFormatter::format(cJSON* json) {
    for (auto& formatter : formatters_) {
        const auto code = formatter->format(json);
        if (code != status::StatusCode::OK) {
            return code;
        }
    }

    return status::StatusCode::OK;
}

void FanoutFormatter::add(IFormatter& formatter) {
    formatters_.emplace_back(&formatter);
}

} // namespace json
} // namespace fmt
} // namespace ocs
