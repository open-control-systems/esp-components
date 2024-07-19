/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "esp_log.h"
#include "esp_wifi.h"

#include "ocs_iot/cjson_object_formatter.h"
#include "ocs_iot/network_json_formatter.h"
#include "ocs_net/ip_addr_to_str.h"
#include "ocs_net/rssi_to_str.h"

namespace ocs {
namespace iot {

namespace {

const char* log_tag = "network-json-formatter";

} // namespace

NetworkJSONFormatter::NetworkJSONFormatter(net::BasicNetwork& network)
    : network_(network) {
}

void NetworkJSONFormatter::format(cJSON* json) {
    format_ap_info_(json);
    format_ip_addr_(json);
}

void NetworkJSONFormatter::format_ap_info_(cJSON* json) {
    wifi_ap_record_t record;
    memset(&record, 0, sizeof(record));

    cJSONObjectFormatter formatter(json);

    const auto err = esp_wifi_sta_get_ap_info(&record);
    if (err == ESP_OK) {
        formatter.add_string_cs("network_ssid",
                                reinterpret_cast<const char*>(record.ssid));

        formatter.add_number_cs("network_rssi", record.rssi);
        formatter.add_string_ref_cs("network_rssi_str", net::rssi_to_str(record.rssi));
    } else {
        ESP_LOGE(log_tag, "esp_wifi_sta_get_ap_info(): %s", esp_err_to_name(err));

        formatter.add_string_ref_cs("network_ssid", "<none>");
        formatter.add_number_cs("network_rssi", 0);
        formatter.add_string_ref_cs("network_rssi_str", "<none>");
    }
}

void NetworkJSONFormatter::format_ip_addr_(cJSON* json) {
    cJSONObjectFormatter formatter(json);

    const auto addr = network_.get_ip_addr();
    if (addr) {
        net::ip_addr_to_str ip_str(*addr);
        formatter.add_string_cs("network_ip", ip_str.c_str());
    } else {
        formatter.add_string_ref_cs("network_ip", "<none>");
    }
}

} // namespace iot
} // namespace ocs
