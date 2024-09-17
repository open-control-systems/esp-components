/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_iot/http_system_handler.h"

namespace ocs {
namespace iot {

namespace {

const char* log_tag = "http-system-handler";

} // namespace

HttpSystemHandler::HttpSystemHandler(net::HttpServer& server,
                                     scheduler::ITask& reboot_task) {
    server.add_GET("/system/reboot", [&reboot_task](httpd_req_t* req) {
        const auto err = httpd_resp_send(req, "Rebooting...", HTTPD_RESP_USE_STRLEN);
        if (err != ESP_OK) {
            return status::StatusCode::Error;
        }

        ocs_logi(log_tag, "Rebooting...");

        return reboot_task.run();
    });
}

} // namespace iot
} // namespace ocs
