/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "esp_wifi.h"

#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_pipeline/jsonfmt/ap_network_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

ApNetworkFormatter::ApNetworkFormatter(net::IApNetwork& network)
    : network_(network) {
}

status::StatusCode ApNetworkFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    const auto info = network_.get_info();

    if (!formatter.add_number_cs("network_ap_channel", info.channel)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("network_ap_cur_conn", info.cur_connection)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("network_ap_max_conn", info.max_connection)) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
