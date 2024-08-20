/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_iot/http_system_state_handler.h"
#include "ocs_iot/cjson_builder.h"
#include "ocs_iot/system_state_json_formatter.h"

namespace ocs {
namespace iot {

HttpSystemStateHandler::HttpSystemStateHandler(net::HttpServer& server,
                                               unsigned response_size) {
    state_json_formatter_.reset(new (std::nothrow) SystemStateJsonFormatter());
    configASSERT(state_json_formatter_);

    json_formatter_.reset(new (std::nothrow) DynamicJsonFormatter(response_size));
    configASSERT(json_formatter_);

    server.add_GET("/report/system", [this](httpd_req_t* req) {
        auto json = CjsonUniqueBuilder::make_json();
        if (!json) {
            return status::StatusCode::NoMem;
        }

        state_json_formatter_->format(json.get());
        json_formatter_->format(json.get());

        const auto err =
            httpd_resp_send(req, json_formatter_->c_str(), HTTPD_RESP_USE_STRLEN);
        if (err != ESP_OK) {
            return status::StatusCode::Error;
        }

        return status::StatusCode::OK;
    });
}

} // namespace iot
} // namespace ocs
