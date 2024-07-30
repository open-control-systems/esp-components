/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "esp_log.h"

#include "ocs_net/http_server.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "http-server";

} // namespace

HttpServer::HttpServer(const Params& params) {
    config_ = HTTPD_DEFAULT_CONFIG();
    config_.server_port = params.server_port;
}

HttpServer::~HttpServer() {
    if (handle_) {
        stop();
    }
}

void HttpServer::add_GET(const char* path, HttpServer::HandlerFunc func) {
    auto& handler = uris_get_[path];
    memset(&handler.uri, 0, sizeof(handler.uri));

    handler.uri.method = HTTP_GET;
    handler.uri.handler = handle_request_;
    handler.uri.user_ctx = this;

    handler.func = func;
}

status::StatusCode HttpServer::start() {
    const auto err = httpd_start(&handle_, &config_);
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "httpd_start(): %s", esp_err_to_name(err));
        return status::StatusCode::Error;
    }

    return register_uris_();
}

status::StatusCode HttpServer::register_uris_() {
    for (auto& [path, handler] : uris_get_) {
        handler.uri.uri = path.c_str();
        ESP_ERROR_CHECK(httpd_register_uri_handler(handle_, &handler.uri));
    }

    return status::StatusCode::OK;
}

status::StatusCode HttpServer::stop() {
    if (handle_) {
        ESP_ERROR_CHECK(httpd_stop(handle_));
        handle_ = nullptr;
    }

    return status::StatusCode::OK;
}

esp_err_t HttpServer::handle_request_(httpd_req_t* req) {
    HttpServer& self = *static_cast<HttpServer*>(req->user_ctx);
    self.handle_request_get_(req);

    return ESP_OK;
}

void HttpServer::handle_request_get_(httpd_req_t* req) {
    const auto handler = uris_get_.find(req->uri);
    if (handler == uris_get_.end()) {
        ESP_LOGE(log_tag, "unknown URI: %s", req->uri);
        return;
    }

    const auto code = handler->second.func(req);
    if (code != status::StatusCode::OK) {
        ESP_LOGE(log_tag, "failed to handle request: URI=%s code=%s", req->uri,
                 status::code_to_str(code));
    }
}

} // namespace net
} // namespace ocs
