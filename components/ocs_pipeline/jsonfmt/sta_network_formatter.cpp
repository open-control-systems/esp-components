/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_net/ip_addr_to_str.h"
#include "ocs_net/rssi_to_signal_strength.h"
#include "ocs_pipeline/jsonfmt/sta_network_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

StaNetworkFormatter::StaNetworkFormatter(net::IStaNetwork& network)
    : network_(network) {
}

status::StatusCode StaNetworkFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    const auto info = network_.get_info();

    if (!formatter.add_string_cs("network_sta_ssid", info.ssid.c_str())) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("network_sta_rssi", info.rssi)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_string_ref_cs("network_sta_signal_strength",
                                     net::rssi_to_signal_strength(info.rssi))) {
        return status::StatusCode::NoMem;
    }

    net::ip_addr_to_str ip_str(info.ip_addr);
    if (!formatter.add_string_cs("network_sta_ip", ip_str.c_str())) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
