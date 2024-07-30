/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_iot/counter_json_formatter.h"
#include "ocs_iot/cjson_object_formatter.h"

namespace ocs {
namespace iot {

void CounterJSONFormatter::format(cJSON* json) {
    CjsonObjectFormatter formatter(json);

    for (auto& counter : get_counters_()) {
        formatter.add_number_cs(counter->id(), counter->get());
    }
}

} // namespace iot
} // namespace ocs
