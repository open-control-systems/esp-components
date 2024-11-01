/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/httpserver/server_pipeline.h"
#include "ocs_core/log.h"
#include "ocs_net/wifi_network.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

namespace {

const char* log_tag = "http_server_pipeline";

} // namespace

ServerPipeline::ServerPipeline() {
    wifi_network_.reset(new (std::nothrow) net::WiFiNetwork(net::WiFiNetwork::Params {
        .max_retry_count = CONFIG_OCS_NETWORK_WIFI_STA_RETRY_COUNT,
        .ssid = CONFIG_OCS_NETWORK_WIFI_STA_SSID,
        .password = CONFIG_OCS_NETWORK_WIFI_STA_PASSWORD,
    }));
    configASSERT(wifi_network_);

    wifi_network_->add(*this);

    http_server_.reset(new (std::nothrow) http::Server(http::Server::Params {
        .server_port = CONFIG_OCS_HTTP_SERVER_PORT,
        .max_uri_handlers = CONFIG_OCS_HTTP_SERVER_MAX_URI_HANDLERS,
    }));
    configASSERT(http_server_);

    mdns_provider_.reset(new (std::nothrow) net::MdnsProvider(net::MdnsProvider::Params {
        .hostname = CONFIG_OCS_NETWORK_MDNS_HOSTNAME,
        .instance_name = CONFIG_OCS_NETWORK_MDNS_INSTANCE_NAME,
    }));
    configASSERT(mdns_provider_);
}

void ServerPipeline::handle_connect() {
    const auto code = http_server_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start HTTP server when WiFi is connected: code=%s",
                 status::code_to_str(code));
    }
}

void ServerPipeline::handle_disconnect() {
    const auto code = http_server_->stop();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to stop HTTP server when WiFi is disconnected: code=%s",
                 status::code_to_str(code));
    }
}

status::StatusCode ServerPipeline::handle_suspend() {
    return mdns_provider_->stop();
}

status::StatusCode ServerPipeline::handle_resume() {
    return try_start_mdns_();
}

status::StatusCode ServerPipeline::start() {
    auto code = try_start_wifi_();
    if (code != status::StatusCode::OK) {
        stop_wifi_();
        return code;
    }

    code = try_start_mdns_();
    if (code != status::StatusCode::OK) {
        return mdns_provider_->stop();
    }

    return status::StatusCode::OK;
}

net::BasicNetwork& ServerPipeline::network() {
    return *wifi_network_;
}

http::Server& ServerPipeline::server() {
    return *http_server_;
}

net::MdnsProvider& ServerPipeline::mdns() {
    return *mdns_provider_;
}

status::StatusCode ServerPipeline::try_start_wifi_() {
    auto code = wifi_network_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start the WiFi connection process: code=%s",
                 status::code_to_str(code));

        return code;
    }

    code = wifi_network_->wait();
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode ServerPipeline::try_start_mdns_() {
    auto code = mdns_provider_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start the mDNS service: code=%s",
                 status::code_to_str(code));

        return code;
    }

    code = mdns_provider_->add_service(net::MdnsProvider::Service::Http,
                                       net::MdnsProvider::Proto::Tcp,
                                       CONFIG_OCS_HTTP_SERVER_PORT);
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

void ServerPipeline::stop_wifi_() {
    const auto code = wifi_network_->stop();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to stop the WiFi connection process: code=%s",
                 status::code_to_str(code));
    }
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
