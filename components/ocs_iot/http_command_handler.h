/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "esp_log.h"

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/noncopyable.h"
#include "ocs_iot/cjson_array_formatter.h"
#include "ocs_iot/cjson_builder.h"
#include "ocs_iot/default_json_formatter.h"
#include "ocs_iot/http_command_handler.h"
#include "ocs_net/http_server.h"
#include "ocs_scheduler/itask.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace iot {

template <unsigned CommandBufferSize>
class HttpCommandHandler : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register HTTP commands.
    //!  - @p reboot_task to initiate the reboot process.
    //!  - @p control_task to trigger the control system.
    HttpCommandHandler(net::HttpServer& server,
                       scheduler::ITask& reboot_task,
                       scheduler::ITask& control_task) {
        commands_response_.reset(new (std::nothrow) JsonFormatter());
        configASSERT(commands_response_);

        format_commands_response_();
        register_routes_(server, reboot_task, control_task);
    }

private:
    void format_commands_response_() {
        auto json = iot::CjsonUniqueBuilder::make_json();
        configASSERT(json);

        auto array = cJSON_AddArrayToObject(json.get(), "commands");
        configASSERT(array);

        iot::CjsonArrayFormatter formatter(array);

        configASSERT(formatter.append_string_ref("reboot"));
        configASSERT(formatter.append_string_ref("reload"));

        commands_response_->format(json.get());
    }

    void register_routes_(net::HttpServer& server,
                          scheduler::ITask& reboot_task,
                          scheduler::ITask& control_task) {
        server.add_GET("/commands/reboot", [&reboot_task](httpd_req_t* req) {
            const auto err = httpd_resp_send(req, "Rebooting...", HTTPD_RESP_USE_STRLEN);
            if (err != ESP_OK) {
                return status::StatusCode::Error;
            }

            ESP_LOGI(log_tag_, "Rebooting...");

            return reboot_task.run();
        });
        server.add_GET("/commands/reload", [&control_task](httpd_req_t* req) {
            const auto err = httpd_resp_send(req, "Reloading...", HTTPD_RESP_USE_STRLEN);
            if (err != ESP_OK) {
                return status::StatusCode::Error;
            }

            ESP_LOGI(log_tag_, "Reloading...");

            return control_task.run();
        });
        server.add_GET("/commands", [this](httpd_req_t* req) {
            const auto err =
                httpd_resp_send(req, commands_response_->c_str(), HTTPD_RESP_USE_STRLEN);
            if (err != ESP_OK) {
                return status::StatusCode::Error;
            }

            return status::StatusCode::OK;
        });
    }

    using JsonFormatter = iot::DefaultJsonFormatter<CommandBufferSize>;

    static const constexpr char* log_tag_ = "http-command-handler";

    std::unique_ptr<JsonFormatter> commands_response_;
};

} // namespace iot
} // namespace ocs
