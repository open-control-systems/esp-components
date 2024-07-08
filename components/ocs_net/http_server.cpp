#include <cstring>

#include "esp_log.h"

#include "ocs_net/http_server.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "http-server";

} // namespace

HTTPServer::HTTPServer(const Params& params) {
    config_ = HTTPD_DEFAULT_CONFIG();
    config_.server_port = params.server_port;
}

HTTPServer::~HTTPServer() {
    if (handle_) {
        stop();
    }
}

void HTTPServer::add_GET(const char* path, HTTPServer::HandlerFunc func) {
    auto& handler = uris_get_[path];
    memset(&handler.uri, 0, sizeof(handler.uri));

    handler.uri.method = HTTP_GET;
    handler.uri.handler = handle_request_;
    handler.uri.user_ctx = this;

    handler.func = func;
}

status::StatusCode HTTPServer::start() {
    const auto err = httpd_start(&handle_, &config_);
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "httpd_start(): %s", esp_err_to_name(err));
        return status::StatusCode::Error;
    }

    return register_uris_();
}

status::StatusCode HTTPServer::register_uris_() {
    for (auto& [path, handler] : uris_get_) {
        handler.uri.uri = path.c_str();
        ESP_ERROR_CHECK(httpd_register_uri_handler(handle_, &handler.uri));
    }

    return status::StatusCode::OK;
}

status::StatusCode HTTPServer::stop() {
    if (handle_) {
        ESP_ERROR_CHECK(httpd_stop(handle_));
        handle_ = nullptr;
    }

    return status::StatusCode::OK;
}

esp_err_t HTTPServer::handle_request_(httpd_req_t* req) {
    HTTPServer& self = *static_cast<HTTPServer*>(req->user_ctx);
    self.handle_request_get_(req);

    return ESP_OK;
}

void HTTPServer::handle_request_get_(httpd_req_t* req) {
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
