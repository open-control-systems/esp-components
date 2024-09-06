/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "esp_log.h"

#include "ocs_iot/http_pipeline.h"
#include "ocs_iot/network_json_formatter.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace iot {

namespace {

const char* log_tag = "http-pipeline";

} // namespace

HttpPipeline::HttpPipeline(scheduler::ITask& reboot_task,
                           IJsonFormatter& telemetry_formatter,
                           FanoutJsonFormatter& registration_formatter,
                           Params params) {
    http_server_pipeline_.reset(new (std::nothrow) HttpServerPipeline());
    configASSERT(http_server_pipeline_);

    http_telemetry_handler_.reset(new (std::nothrow) HttpDataHandler(
        http_server_pipeline_->server(), telemetry_formatter, "/telemetry",
        "http-telemetry-handler", params.telemetry.buffer_size));
    configASSERT(http_telemetry_handler_);

    http_registration_handler_.reset(new (std::nothrow) HttpDataHandler(
        http_server_pipeline_->server(), registration_formatter, "/registration",
        "http-registration-handler", params.registration.buffer_size));
    configASSERT(http_registration_handler_);

    http_command_handler_.reset(new (std::nothrow) HttpCommandHandler(
        http_server_pipeline_->server(), reboot_task, params.commands.buffer_size));
    configASSERT(http_command_handler_);

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
        ESP_LOGE(log_tag, "failed to start HTTP server pipeline: code=%s",
                 status::code_to_str(code));

        return code;
    }

    code = register_mdns_endpoints_();
    if (code != status::StatusCode::OK) {
        ESP_LOGE(log_tag, "failed to register mDNS endpoints: code=%s",
                 status::code_to_str(code));

        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode HttpPipeline::register_mdns_endpoints_() {
    http_server_pipeline_->mdns().add_txt_records(net::MdnsProvider::Service::Http,
                                                  net::MdnsProvider::Proto::Tcp,
                                                  net::MdnsProvider::TxtRecordList {
                                                      {
                                                          "telemetry",
                                                          "/telemetry",
                                                      },
                                                      {
                                                          "registration",
                                                          "/registration",
                                                      },
                                                      {
                                                          "command_reboot",
                                                          "/commands/reboot",
                                                      },
                                                      {
                                                          "command_reload",
                                                          "/commands/reload",
                                                      },
                                                      {
                                                          "commands",
                                                          "/commands",
                                                      },
                                                  });

    return http_server_pipeline_->mdns().flush_txt_records();
}

HttpServerPipeline& HttpPipeline::get_server_pipeline() {
    return *http_server_pipeline_;
}

} // namespace iot
} // namespace ocs
