/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "esp_http_server.h"

#include "ocs_core/noncopyable.h"
#include "ocs_status/code.h"

namespace ocs {
namespace net {

class HttpServer : public core::NonCopyable<> {
public:
    //! Handler to process an HTTP request.
    using HandlerFunc = std::function<status::StatusCode(httpd_req_t* req)>;

    struct Params {
        //! TCP port to accept incoming connections.
        unsigned server_port { 80 };
    };

    //! Initialize.
    explicit HttpServer(const Params& params);

    //! Destroy.
    ~HttpServer();

    //! Start HTTP server.
    status::StatusCode start();

    //! Stop HTTP server.
    //!
    //! @remarks
    //!  Can be called multiple times.
    status::StatusCode stop();

    //! Register HTTP handler for a GET request.
    void add_GET(const char* path, HandlerFunc func);

private:
    struct Handler {
        httpd_uri_t uri {};
        HandlerFunc func { nullptr };
    };

    using URIMap = std::unordered_map<std::string, Handler>;

    static esp_err_t handle_request_(httpd_req_t* req);

    status::StatusCode register_uris_();

    void handle_request_get_(httpd_req_t* req);

    httpd_handle_t handle_ { nullptr };
    httpd_config_t config_;

    URIMap uris_get_;
};

} // namespace net
} // namespace ocs
