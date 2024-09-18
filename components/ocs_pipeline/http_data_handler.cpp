/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/http_data_handler.h"
#include "ocs_core/log.h"
#include "ocs_fmt/json/cjson_builder.h"

namespace ocs {
namespace pipeline {

HttpDataHandler::HttpDataHandler(net::HttpServer& server,
                                 net::MdnsProvider& provider,
                                 fmt::json::IFormatter& formatter,
                                 const char* path,
                                 const char* id,
                                 unsigned buffer_size) {
    fanout_formatter_.reset(new (std::nothrow) fmt::json::FanoutFormatter());
    configASSERT(fanout_formatter_);

    fanout_formatter_->add(formatter);

    json_formatter_.reset(new (std::nothrow) fmt::json::DynamicFormatter(buffer_size));
    configASSERT(json_formatter_);

    fanout_formatter_->add(*json_formatter_);

    server.add_GET(path, [this, path, id](httpd_req_t* req) {
        auto json = fmt::json::CjsonUniqueBuilder::make_object();
        if (!json) {
            return status::StatusCode::NoMem;
        }

        const auto code = fanout_formatter_->format(json.get());
        if (code != status::StatusCode::OK) {
            return code;
        }

        const auto err =
            httpd_resp_send(req, json_formatter_->c_str(), HTTPD_RESP_USE_STRLEN);
        if (err != ESP_OK) {
            ocs_loge(id, "httpd_resp_send(): %s", esp_err_to_name(err));
            return status::StatusCode::Error;
        }

        return status::StatusCode::OK;
    });
    provider.add_txt_records(net::MdnsProvider::Service::Http,
                             net::MdnsProvider::Proto::Tcp,
                             net::MdnsProvider::TxtRecordList {
                                 {
                                     id,
                                     path,
                                 },
                             });
}

} // namespace pipeline
} // namespace ocs
