/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/httpserver/time_handler.h"
#include "ocs_algo/time_ops.h"
#include "ocs_algo/uri_ops.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

TimeHandler::TimeHandler(http::Server& server, time_t start_point) {
    server.add_GET("/api/v1/system/time", [start_point](httpd_req_t* req) {
        const auto values = algo::UriOps::parse_query(req->uri);
        const auto it = values.find("value");

        if (it == values.end()) {
            const auto timestamp = algo::TimeOps::get_time();
            if (!timestamp) {
                return status::StatusCode::Error;
            }

            auto err = httpd_resp_set_type(req, HTTPD_TYPE_TEXT);
            if (err != ESP_OK) {
                return status::StatusCode::Error;
            }

            err = httpd_resp_send(req, std::to_string(*timestamp).c_str(),
                                  HTTPD_RESP_USE_STRLEN);
            if (err != ESP_OK) {
                return status::StatusCode::Error;
            }

            return status::StatusCode::OK;
        }

        char buf[it->second.size() + 1];
        memset(buf, 0, sizeof(buf));
        memcpy(buf, it->second.data(), it->second.size());

        const auto code = algo::TimeOps::set_time(buf, start_point);
        if (code != status::StatusCode::OK) {
            return code;
        }

        auto err = httpd_resp_set_type(req, HTTPD_TYPE_TEXT);
        if (err != ESP_OK) {
            return status::StatusCode::Error;
        }

        err = httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
        if (err != ESP_OK) {
            return status::StatusCode::Error;
        }

        return status::StatusCode::OK;
    });
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
