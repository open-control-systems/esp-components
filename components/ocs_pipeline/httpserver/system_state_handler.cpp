/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/httpserver/system_state_handler.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_pipeline/jsonfmt/system_state_formatter.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

SystemStateHandler::SystemStateHandler(http::Server& server,
                                       net::IMdnsDriver& mdns_driver,
                                       unsigned response_size) {
    state_json_formatter_.reset(new (std::nothrow) jsonfmt::SystemStateFormatter());
    configASSERT(state_json_formatter_);

    json_formatter_.reset(new (std::nothrow) fmt::json::DynamicFormatter(response_size));
    configASSERT(json_formatter_);

    server.add_GET("/api/v1/system/report", [this](httpd_req_t* req) {
        auto json = fmt::json::CjsonUniqueBuilder::make_object();
        if (!json) {
            return status::StatusCode::NoMem;
        }

        auto code = state_json_formatter_->format(json.get());
        if (code != status::StatusCode::OK) {
            return code;
        }

        code = json_formatter_->format(json.get());
        if (code != status::StatusCode::OK) {
            return code;
        }

        const auto err =
            httpd_resp_send(req, json_formatter_->c_str(), HTTPD_RESP_USE_STRLEN);
        if (err != ESP_OK) {
            return status::StatusCode::Error;
        }

        return status::StatusCode::OK;
    });

    configASSERT(mdns_driver.add_txt_record(
                     net::IMdnsDriver::Service::Http, net::IMdnsDriver::Proto::Tcp,
                     "api_v1_system_report", "/api/v1/system/report")
                 == status::StatusCode::OK);
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
