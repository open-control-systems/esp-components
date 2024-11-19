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
#include "ocs_pipeline/basic/network_pipeline.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace pipeline {
namespace basic {

namespace {

const char* log_tag = "network_pipeline";

} // namespace

NetworkPipeline::NetworkPipeline(storage::StorageBuilder& storage_builder,
                                 system::DeviceID& device_id) {
    storage_ = storage_builder.make("net_wifi");
    configASSERT(storage_);

    initialize_nework_(device_id);
}

status::StatusCode NetworkPipeline::start() {
    const auto code = start_();
    if (code != status::StatusCode::OK) {
        stop_();
    }

    return code;
}

net::BasicNetwork& NetworkPipeline::get_network() {
    return *network_;
}

void NetworkPipeline::initialize_nework_(system::DeviceID& device_id) {
    NetworkType network_type = NetworkType::Ap;

    auto code = read_network_type_(network_type);
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        ocs_loge(log_tag, "failed to read network type from storage: %s",
                 status::code_to_str(code));
    }

    switch (network_type) {
    case NetworkType::Ap:
        initialize_network_ap_(device_id);
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

status::StatusCode NetworkPipeline::read_network_type_(NetworkType& type) {
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

void NetworkPipeline::initialize_network_ap_(system::DeviceID& device_id) {
    char ssid[max_ssid_size_];
    memset(ssid, 0, sizeof(ssid));

    auto code =
        algo::StorageOps::prob_read(*storage_, "net_ap_ssid", ssid, max_ssid_size_);
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read WiFi AP SSID from storage: %s",
                     status::code_to_str(code));
        }

        std::string builtin_ssid = "ocs-ssid-";
        builtin_ssid += device_id.get_id();

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

        std::string builtin_password = "ocs-password-";
        builtin_password += std::string(device_id.get_id(), 7);

        strncpy(password, builtin_password.c_str(), sizeof(password));
    }

    network_.reset(new (std::nothrow) net::ApNetwork(net::ApNetwork::Params {
        .ssid = ssid,
        .password = password,
        .channel = CONFIG_OCS_NETWORK_WIFI_AP_CHANNEL,
        .max_connection = CONFIG_OCS_NETWORK_WIFI_AP_MAX_CONN,
    }));
}

void NetworkPipeline::initialize_network_sta_() {
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

status::StatusCode NetworkPipeline::start_() {
    auto code = network_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start network: code=%s", status::code_to_str(code));

        return code;
    }

    code = network_->wait(wait_start_interval_);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to wait for network: code=%s",
                 status::code_to_str(code));

        return code;
    }

    return status::StatusCode::OK;
}

void NetworkPipeline::stop_() {
    const auto code = network_->stop();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to stop network: code=%s", status::code_to_str(code));
    }
}

} // namespace basic
} // namespace pipeline
} // namespace ocs
