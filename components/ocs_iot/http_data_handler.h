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

#include "ocs_core/noncopyable.h"
#include "ocs_iot/cjson_builder.h"
#include "ocs_iot/default_json_formatter.h"
#include "ocs_iot/fanout_json_formatter.h"
#include "ocs_net/http_server.h"

namespace ocs {
namespace iot {

template <unsigned Size>
class HttpDataHandler : public core::NonCopyable<HttpDataHandler<Size>> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register the HTTP endpoint.
    //!  - @p formatter to format the data.
    //!  - @p path - URI path.
    //!  - @p log_tag - ESP-IDF log tag.
    HttpDataHandler(net::HttpServer& server,
                    IJsonFormatter& formatter,
                    const char* path,
                    const char* log_tag) {
        fanout_formatter_.reset(new (std::nothrow) FanoutJsonFormatter());
        configASSERT(fanout_formatter_);

        fanout_formatter_->add(formatter);

        json_formatter_.reset(new (std::nothrow) JsonFormatter());
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

private:
    using JsonFormatter = DefaultJsonFormatter<Size>;

    std::unique_ptr<FanoutJsonFormatter> fanout_formatter_;
    std::unique_ptr<JsonFormatter> json_formatter_;
};

} // namespace iot
} // namespace ocs
