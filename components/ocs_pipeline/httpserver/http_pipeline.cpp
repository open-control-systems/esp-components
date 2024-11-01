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
                           fmt::json::IFormatter& telemetry_formatter,
                           fmt::json::FanoutFormatter& registration_formatter,
                           Params params) {
    server_pipeline_.reset(new (std::nothrow) ServerPipeline());
    configASSERT(server_pipeline_);

    configASSERT(suspender.add(*server_pipeline_, "http_server_pipeline")
                 == status::StatusCode::OK);

    telemetry_handler_.reset(new (std::nothrow) DataHandler(
        server_pipeline_->server(), server_pipeline_->mdns(), telemetry_formatter,
        "/telemetry", "http_telemetry_handler", params.telemetry.buffer_size));
    configASSERT(telemetry_handler_);

    registration_handler_.reset(new (std::nothrow) DataHandler(
        server_pipeline_->server(), server_pipeline_->mdns(), registration_formatter,
        "/registration", "http_registration_handler", params.registration.buffer_size));
    configASSERT(registration_handler_);

    system_handler_.reset(new (std::nothrow) SystemHandler(
        server_pipeline_->server(), server_pipeline_->mdns(), reboot_task));
    configASSERT(system_handler_);

    network_formatter_.reset(new (std::nothrow)
                                 jsonfmt::NetworkFormatter(server_pipeline_->network()));
    configASSERT(network_formatter_);

    registration_formatter.add(*network_formatter_);

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    system_state_handler_.reset(new (std::nothrow) SystemStateHandler(
        server_pipeline_->server(), server_pipeline_->mdns(), 1024 * 2));
    configASSERT(system_state_handler_);
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY
}

status::StatusCode HttpPipeline::start() {
    auto code = server_pipeline_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start HTTP server pipeline: code=%s",
                 status::code_to_str(code));

        return code;
    }

    code = server_pipeline_->mdns().flush_txt_records();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to register mDNS txt records: code=%s",
                 status::code_to_str(code));

        return code;
    }

    return status::StatusCode::OK;
}

ServerPipeline& HttpPipeline::get_server_pipeline() {
    return *server_pipeline_;
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
