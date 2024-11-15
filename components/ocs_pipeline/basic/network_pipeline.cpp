/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_net/wifi_network.h"
#include "ocs_pipeline/basic/network_pipeline.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace pipeline {
namespace basic {

namespace {

const char* log_tag = "network_pipeline";

} // namespace

NetworkPipeline::NetworkPipeline() {
    network_.reset(new (std::nothrow) net::WiFiNetwork(net::WiFiNetwork::Params {
        .max_retry_count = CONFIG_OCS_NETWORK_WIFI_STA_RETRY_COUNT,
        .ssid = CONFIG_OCS_NETWORK_WIFI_STA_SSID,
        .password = CONFIG_OCS_NETWORK_WIFI_STA_PASSWORD,
    }));
    configASSERT(network_);

    mdns_provider_.reset(new (std::nothrow) net::MdnsProvider(net::MdnsProvider::Params {
        .hostname = CONFIG_OCS_NETWORK_MDNS_HOSTNAME,
        .instance_name = CONFIG_OCS_NETWORK_MDNS_INSTANCE_NAME,
    }));
    configASSERT(mdns_provider_);
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

net::MdnsProvider& NetworkPipeline::get_mdns_provider() {
    return *mdns_provider_;
}

status::StatusCode NetworkPipeline::start_() {
    auto code = network_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start network: code=%s", status::code_to_str(code));

        return code;
    }

    code = network_->wait();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to wait for network: code=%s",
                 status::code_to_str(code));

        return code;
    }

    code = mdns_provider_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start mDNS: code=%s", status::code_to_str(code));

        return code;
    }

    code = mdns_provider_->flush_txt_records();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to register mDNS txt records: code=%s",
                 status::code_to_str(code));

        return code;
    }

    return status::StatusCode::OK;
}

void NetworkPipeline::stop_() {
    auto code = network_->stop();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to stop network: code=%s", status::code_to_str(code));
    }

    code = mdns_provider_->stop();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to stop mDNS: code=%s", status::code_to_str(code));
    }
}

} // namespace basic
} // namespace pipeline
} // namespace ocs
