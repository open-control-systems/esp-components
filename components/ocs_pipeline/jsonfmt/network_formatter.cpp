/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "esp_wifi.h"

#include "ocs_core/log.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_net/ip_addr_to_str.h"
#include "ocs_net/rssi_to_signal_strength.h"
#include "ocs_pipeline/jsonfmt/network_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

namespace {

const char* log_tag = "network_json_formatter";

} // namespace

NetworkFormatter::NetworkFormatter(net::BasicNetwork& network)
    : network_(network) {
}

status::StatusCode NetworkFormatter::format(cJSON* json) {
    if (const auto code = format_ap_info_(json); code != status::StatusCode::OK) {
        return code;
    }

    if (const auto code = format_ip_addr_(json); code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode NetworkFormatter::format_ap_info_(cJSON* json) {
    wifi_ap_record_t record;
    memset(&record, 0, sizeof(record));

    fmt::json::CjsonObjectFormatter formatter(json);

    const auto err = esp_wifi_sta_get_ap_info(&record);
    if (err == ESP_OK) {
        if (!formatter.add_string_cs("network_ssid",
                                     reinterpret_cast<const char*>(record.ssid))) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("network_rssi", record.rssi)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_string_ref_cs("network_signal_strength",
                                         net::rssi_to_signal_strength(record.rssi))) {
            return status::StatusCode::NoMem;
        }
    } else {
        ocs_loge(log_tag, "esp_wifi_sta_get_ap_info(): %s", esp_err_to_name(err));

        if (!formatter.add_string_ref_cs("network_ssid", "<none>")) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_number_cs("network_rssi", 0)) {
            return status::StatusCode::NoMem;
        }

        if (!formatter.add_string_ref_cs("network_signal_strength", "<none>")) {
            return status::StatusCode::NoMem;
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode NetworkFormatter::format_ip_addr_(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    const auto addr = network_.get_ip_addr();
    if (addr) {
        net::ip_addr_to_str ip_str(*addr);
        if (!formatter.add_string_cs("network_ip", ip_str.c_str())) {
            return status::StatusCode::NoMem;
        }
    } else {
        if (!formatter.add_string_ref_cs("network_ip", "<none>")) {
            return status::StatusCode::NoMem;
        }
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
