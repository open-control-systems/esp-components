/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_iot/fanout_json_formatter.h"

namespace ocs {
namespace iot {

void FanoutJSONFormatter::format(cJSON* json) {
    for (auto& formatter : formatters_) {
        formatter->format(json);
    }
}

void FanoutJSONFormatter::add(IJSONFormatter& formatter) {
    formatters_.emplace_back(&formatter);
}

} // namespace iot
} // namespace ocs
