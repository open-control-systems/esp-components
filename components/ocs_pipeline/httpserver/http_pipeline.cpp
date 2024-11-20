/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/httpserver/http_pipeline.h"
#include "ocs_core/log.h"
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
                           net::FanoutNetworkHandler& network_handler,
                           net::BasicNetwork& network,
                           net::MdnsProvider& mdns_provider,
                           fmt::json::IFormatter& telemetry_formatter,
                           fmt::json::FanoutFormatter& registration_formatter,
                           Params params)
    : mdns_provider_(mdns_provider) {
    mdns_provider_.add_service(net::MdnsProvider::Service::Http,
                               net::MdnsProvider::Proto::Tcp,
                               CONFIG_OCS_HTTP_SERVER_PORT);

    network_handler.add(*this);

    http_server_.reset(new (std::nothrow) http::Server(http::Server::Params {
        .server_port = CONFIG_OCS_HTTP_SERVER_PORT,
        .max_uri_handlers = CONFIG_OCS_HTTP_SERVER_MAX_URI_HANDLERS,
    }));
    configASSERT(http_server_);

    configASSERT(suspender.add(*this, "http_pipeline") == status::StatusCode::OK);

    telemetry_handler_.reset(new (std::nothrow) DataHandler(
        *http_server_, mdns_provider_, telemetry_formatter, "/telemetry",
        "http_telemetry_handler", params.telemetry.buffer_size));
    configASSERT(telemetry_handler_);

    registration_handler_.reset(new (std::nothrow) DataHandler(
        *http_server_, mdns_provider_, registration_formatter, "/registration",
        "http_registration_handler", params.registration.buffer_size));
    configASSERT(registration_handler_);

    system_handler_.reset(new (std::nothrow)
                              SystemHandler(*http_server_, mdns_provider_, reboot_task));
    configASSERT(system_handler_);

    network_formatter_.reset(new (std::nothrow) jsonfmt::NetworkFormatter(network));
    configASSERT(network_formatter_);

    registration_formatter.add(*network_formatter_);

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    system_state_handler_.reset(
        new (std::nothrow) SystemStateHandler(*http_server_, mdns_provider_, 1024 * 2));
    configASSERT(system_state_handler_);
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY
}

void HttpPipeline::handle_connect() {
    const auto code = http_server_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start HTTP server on network connect: code=%s",
                 status::code_to_str(code));
    }
}

void HttpPipeline::handle_disconnect() {
    const auto code = http_server_->stop();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag,
                 "failed to stop HTTP server when on network disconnect: code=%s",
                 status::code_to_str(code));
    }
}

status::StatusCode HttpPipeline::handle_suspend() {
    return mdns_provider_.stop();
}

status::StatusCode HttpPipeline::handle_resume() {
    return mdns_provider_.start();
}

http::Server& HttpPipeline::get_server() {
    return *http_server_;
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
