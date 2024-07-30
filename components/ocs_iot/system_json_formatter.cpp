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

namespace ocs {
namespace iot {

namespace {

const char* reset_reason_to_str(esp_reset_reason_t reason) {
    switch (reason) {
    case ESP_RST_UNKNOWN:
        // Reset reason can not be determined
        return "RST_UNKNOWN";

    case ESP_RST_POWERON:
        // Reset due to power-on event
        return "RST_POWERON";

    case ESP_RST_EXT:
        // Reset by external pin (not applicable for ESP32)
        return "RST_EXT";

    case ESP_RST_SW:
        // Software reset via esp_restart
        return "RST_SW";

    case ESP_RST_PANIC:
        // Software reset due to exception/panic
        return "RST_PANIC";

    case ESP_RST_INT_WDT:
        // Reset (software or hardware) due to interrupt watchdog
        return "RST_INT_WDT";

    case ESP_RST_TASK_WDT:
        // Reset due to task watchdog
        return "RST_TASK_WDT";

    case ESP_RST_WDT:
        // Reset due to other watchdogs
        return "RST_WDT";

    case ESP_RST_DEEPSLEEP:
        // Reset after exiting deep sleep mode
        return "RST_DEEPSLEEP";

    case ESP_RST_BROWNOUT:
        // Brownout reset (software or hardware)
        return "RST_BROWNOUT";

    case ESP_RST_SDIO:
        // Reset over SDIO
        return "RST_SDIO";

    case ESP_RST_USB:
        // Reset by USB peripheral
        return "RST_USB";

    case ESP_RST_JTAG:
        // Reset by JTAG
        return "RST_JTAG";

    default:
        break;
    }

    return "<none>";
}

} // namespace

void SystemJsonFormatter::format(cJSON* json) {
    CjsonObjectFormatter formatter(json);

    formatter.add_number_cs("system_memory_heap", esp_get_free_heap_size());
    formatter.add_number_cs("system_memory_heap_min", esp_get_minimum_free_heap_size());

    formatter.add_number_cs("system_memory_heap_internal",
                            esp_get_free_internal_heap_size());

    formatter.add_string_ref_cs("system_reset_reason",
                                reset_reason_to_str(esp_reset_reason()));
}

} // namespace iot
} // namespace ocs
