/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/http_pipeline.h"
#include "ocs_core/log.h"
#include "ocs_pipeline/network_json_formatter.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace pipeline {

namespace {

const char* log_tag = "http-pipeline";

} // namespace

HttpPipeline::HttpPipeline(scheduler::ITask& reboot_task,
                           system::FanoutSuspender& suspender,
                           fmt::json::IFormatter& telemetry_formatter,
                           fmt::json::FanoutFormatter& registration_formatter,
                           Params params) {
    http_server_pipeline_.reset(new (std::nothrow) HttpServerPipeline());
    configASSERT(http_server_pipeline_);

    configASSERT(suspender.add(*http_server_pipeline_, "http-server-pipeline")
                 == status::StatusCode::OK);

    http_telemetry_handler_.reset(new (std::nothrow) HttpDataHandler(
        http_server_pipeline_->server(), http_server_pipeline_->mdns(),
        telemetry_formatter, "/telemetry", "http-telemetry-handler",
        params.telemetry.buffer_size));
    configASSERT(http_telemetry_handler_);

    http_registration_handler_.reset(new (std::nothrow) HttpDataHandler(
        http_server_pipeline_->server(), http_server_pipeline_->mdns(),
        registration_formatter, "/registration", "http-registration-handler",
        params.registration.buffer_size));
    configASSERT(http_registration_handler_);

    http_system_handler_.reset(new (std::nothrow) HttpSystemHandler(
        http_server_pipeline_->server(), http_server_pipeline_->mdns(), reboot_task));
    configASSERT(http_system_handler_);

    network_formatter_.reset(new (std::nothrow)
                                 NetworkJsonFormatter(http_server_pipeline_->network()));
    configASSERT(network_formatter_);

    registration_formatter.add(*network_formatter_);

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    http_system_state_handler_.reset(new (std::nothrow) HttpSystemStateHandler(
        http_server_pipeline_->server(), http_server_pipeline_->mdns(), 1024 * 2));
    configASSERT(http_system_state_handler_);
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY
}

status::StatusCode HttpPipeline::start() {
    auto code = http_server_pipeline_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start HTTP server pipeline: code=%s",
                 status::code_to_str(code));

        return code;
    }

    code = http_server_pipeline_->mdns().flush_txt_records();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to register mDNS txt records: code=%s",
                 status::code_to_str(code));

        return code;
    }

    return status::StatusCode::OK;
}

HttpServerPipeline& HttpPipeline::get_server_pipeline() {
    return *http_server_pipeline_;
}

} // namespace pipeline
} // namespace ocs
