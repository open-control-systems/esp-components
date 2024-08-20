/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "esp_system.h"

#include "ocs_iot/cjson_object_formatter.h"
#include "ocs_iot/system_json_formatter.h"
#include "ocs_system/reset_reason_to_str.h"

namespace ocs {
namespace iot {

void SystemJsonFormatter::format(cJSON* json) {
    CjsonObjectFormatter formatter(json);

    formatter.add_number_cs("system_memory_heap", esp_get_free_heap_size());
    formatter.add_number_cs("system_memory_heap_min", esp_get_minimum_free_heap_size());

    formatter.add_number_cs("system_memory_heap_internal",
                            esp_get_free_internal_heap_size());

    formatter.add_string_ref_cs("system_reset_reason",
                                system::reset_reason_to_str(esp_reset_reason()));
}

} // namespace iot
} // namespace ocs
