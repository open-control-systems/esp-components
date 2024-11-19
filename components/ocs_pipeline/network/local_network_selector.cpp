/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>
#include <string>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/storage_ops.h"
#include "ocs_core/log.h"
#include "ocs_net/ap_network.h"
#include "ocs_net/sta_network.h"
#include "ocs_pipeline/network/local_network_selector.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace pipeline {
namespace network {

namespace {

const char* log_tag = "local_network_selector";

} // namespace

LocalNetworkSelector::LocalNetworkSelector(storage::StorageBuilder& storage_builder,
                                           const system::DeviceInfo& device_info) {
    storage_ = storage_builder.make("net_wifi");
    configASSERT(storage_);

    initialize_nework_(device_info);
}

net::BasicNetwork& LocalNetworkSelector::get_network() {
    return *network_;
}

void LocalNetworkSelector::initialize_nework_(const system::DeviceInfo& device_info) {
    NetworkType network_type = NetworkType::Ap;

    auto code = read_network_type_(network_type);
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        ocs_loge(log_tag, "failed to read network type from storage: %s",
                 status::code_to_str(code));
    }

    switch (network_type) {
    case NetworkType::Ap:
        initialize_network_ap_(device_info);
        break;

    case NetworkType::Sta:
        initialize_network_sta_();
        break;

    default:
        //! Should not happen.
        configASSERT(false);
        break;
    }

    configASSERT(network_);
}

status::StatusCode LocalNetworkSelector::read_network_type_(NetworkType& type) {
    unsigned network_type = 0;

    auto code = storage_->read("net_type", &network_type, sizeof(network_type));
    if (code != status::StatusCode::OK) {
        return code;
    }

    if (network_type >= static_cast<unsigned>(NetworkType::Last)) {
        return status::StatusCode::InvalidState;
    }

    type = static_cast<NetworkType>(network_type);

    return status::StatusCode::OK;
}

void LocalNetworkSelector::initialize_network_ap_(const system::DeviceInfo& device_info) {
    char ssid[max_ssid_size_];
    memset(ssid, 0, sizeof(ssid));

    auto code =
        algo::StorageOps::prob_read(*storage_, "net_ap_ssid", ssid, max_ssid_size_);
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read WiFi AP SSID from storage: %s",
                     status::code_to_str(code));
        }

        std::string builtin_ssid = device_info.get_fw_name();
        builtin_ssid += "-";
        builtin_ssid += device_info.get_device_id();

        strncpy(ssid, builtin_ssid.c_str(), sizeof(ssid));
    }

    char password[max_password_size_];
    memset(password, 0, sizeof(password));

    code = algo::StorageOps::prob_read(*storage_, "net_ap_pswd", password,
                                       max_password_size_);
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read WiFi AP password from storage: %s",
                     status::code_to_str(code));
        }

        std::string builtin_password = device_info.get_fw_name();
        builtin_password += "-";
        builtin_password += std::string(device_info.get_device_id(), 7);

        strncpy(password, builtin_password.c_str(), sizeof(password));
    }

    network_.reset(new (std::nothrow) net::ApNetwork(net::ApNetwork::Params {
        .ssid = ssid,
        .password = password,
        .channel = CONFIG_OCS_NETWORK_WIFI_AP_CHANNEL,
        .max_connection = CONFIG_OCS_NETWORK_WIFI_AP_MAX_CONN,
    }));
}

void LocalNetworkSelector::initialize_network_sta_() {
    char ssid[max_ssid_size_];
    memset(ssid, 0, sizeof(ssid));

    auto code =
        algo::StorageOps::prob_read(*storage_, "net_sta_ssid", ssid, max_ssid_size_);
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        ocs_loge(log_tag, "failed to read WiFi STA SSID from storage: %s",
                 status::code_to_str(code));
    }

    char password[max_password_size_];
    memset(password, 0, sizeof(password));

    code = algo::StorageOps::prob_read(*storage_, "net_sta_pswd", password,
                                       max_password_size_);
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        ocs_loge(log_tag, "failed to read WiFi STA password from storage: %s",
                 status::code_to_str(code));
    }

    network_.reset(new (std::nothrow) net::StaNetwork(net::StaNetwork::Params {
        .max_retry_count = CONFIG_OCS_NETWORK_WIFI_STA_RETRY_COUNT,
        .ssid = ssid,
        .password = password,
    }));
}

} // namespace network
} // namespace pipeline
} // namespace ocs
