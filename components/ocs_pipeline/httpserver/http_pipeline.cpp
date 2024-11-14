/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/httpserver/http_pipeline.h"
#include "ocs_core/log.h"
#include "ocs_net/wifi_network.h"
#include "ocs_pipeline/jsonfmt/network_formatter.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

namespace {

const char* log_tag = "http_pipeline";

} // namespace

HttpPipeline::HttpPipeline(scheduler::ITask& reboot_task,
                           system::FanoutSuspender& suspender,
                           fmt::json::IFormatter& telemetry_formatter,
                           fmt::json::FanoutFormatter& registration_formatter,
                           Params params) {
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

    configASSERT(suspender.add(*this, "http_pipeline") == status::StatusCode::OK);

    telemetry_handler_.reset(new (std::nothrow) DataHandler(
        *http_server_, *mdns_provider_, telemetry_formatter, "/telemetry",
        "http_telemetry_handler", params.telemetry.buffer_size));
    configASSERT(telemetry_handler_);

    registration_handler_.reset(new (std::nothrow) DataHandler(
        *http_server_, *mdns_provider_, registration_formatter, "/registration",
        "http_registration_handler", params.registration.buffer_size));
    configASSERT(registration_handler_);

    system_handler_.reset(new (std::nothrow)
                              SystemHandler(*http_server_, *mdns_provider_, reboot_task));
    configASSERT(system_handler_);

    network_formatter_.reset(new (std::nothrow)
                                 jsonfmt::NetworkFormatter(*wifi_network_));
    configASSERT(network_formatter_);

    registration_formatter.add(*network_formatter_);

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    system_state_handler_.reset(
        new (std::nothrow) SystemStateHandler(*http_server_, *mdns_provider_, 1024 * 2));
    configASSERT(system_state_handler_);
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY
}

void HttpPipeline::handle_connect() {
    const auto code = http_server_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start HTTP server when WiFi is connected: code=%s",
                 status::code_to_str(code));
    }
}

void HttpPipeline::handle_disconnect() {
    const auto code = http_server_->stop();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to stop HTTP server when WiFi is disconnected: code=%s",
                 status::code_to_str(code));
    }
}

status::StatusCode HttpPipeline::handle_suspend() {
    return mdns_provider_->stop();
}

status::StatusCode HttpPipeline::handle_resume() {
    return try_start_mdns_();
}

status::StatusCode HttpPipeline::start() {
    auto code = try_start_wifi_();
    if (code != status::StatusCode::OK) {
        stop_wifi_();
        return code;
    }

    code = try_start_mdns_();
    if (code != status::StatusCode::OK) {
        return mdns_provider_->stop();
    }

    code = mdns_provider_->flush_txt_records();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to register mDNS txt records: code=%s",
                 status::code_to_str(code));

        return code;
    }

    return status::StatusCode::OK;
}

net::BasicNetwork& HttpPipeline::get_network() {
    return *wifi_network_;
}

http::Server& HttpPipeline::get_server() {
    return *http_server_;
}

net::MdnsProvider& HttpPipeline::get_mdns_provider() {
    return *mdns_provider_;
}

status::StatusCode HttpPipeline::try_start_wifi_() {
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

status::StatusCode HttpPipeline::try_start_mdns_() {
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

void HttpPipeline::stop_wifi_() {
    const auto code = wifi_network_->stop();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to stop the WiFi connection process: code=%s",
                 status::code_to_str(code));
    }
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
