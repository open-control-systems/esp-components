/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "esp_log.h"

#include "ocs_iot/cjson_builder.h"
#include "ocs_iot/http_data_handler.h"

namespace ocs {
namespace iot {

HttpDataHandler::HttpDataHandler(net::HttpServer& server,
                                 IJsonFormatter& formatter,
                                 const char* path,
                                 const char* log_tag,
                                 unsigned buffer_size) {
    fanout_formatter_.reset(new (std::nothrow) FanoutJsonFormatter());
    configASSERT(fanout_formatter_);

    fanout_formatter_->add(formatter);

    json_formatter_.reset(new (std::nothrow) DynamicJsonFormatter(buffer_size));
    configASSERT(json_formatter_);

    fanout_formatter_->add(*json_formatter_);

    server.add_GET(path, [this, path, log_tag](httpd_req_t* req) {
        auto json = CjsonUniqueBuilder::make_json();
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
            ESP_LOGE(log_tag, "httpd_resp_send(): %s", esp_err_to_name(err));
            return status::StatusCode::Error;
        }

        return status::StatusCode::OK;
    });
}

} // namespace iot
} // namespace ocs
