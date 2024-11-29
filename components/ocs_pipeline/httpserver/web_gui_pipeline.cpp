/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>
#include <string_view>

#include "esp_spiffs.h"

#include "ocs_core/file_stream_reader.h"
#include "ocs_core/log.h"
#include "ocs_http/chunk_stream_writer.h"
#include "ocs_pipeline/httpserver/web_gui_pipeline.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

namespace {

bool check_file_extension(const char* filepath, const char* ext) {
    return std::string_view(filepath).ends_with(ext);
}

const char* parse_content_type(const char* filepath) {
    if (check_file_extension(filepath, ".html")) {
        return "text/html";
    }

    if (check_file_extension(filepath, ".js")) {
        return "application/javascript";
    }

    if (check_file_extension(filepath, ".css")) {
        return "text/css";
    }

    if (check_file_extension(filepath, ".png")) {
        return "image/png";
    }

    if (check_file_extension(filepath, ".ico")) {
        return "image/x-icon";
    }

    if (check_file_extension(filepath, ".svg")) {
        return "text/xml";
    }

    return "text/plain";
}

const char* log_tag = "web_gui_pipeline";

} // namespace

WebGuiPipeline::WebGuiPipeline(http::Server& server) {
    buffer_.resize(buffer_size_);

    initialize_fs_();

    server.add_GET("/", [this](httpd_req_t* req) {
        return handle_file_(req);
    });
    server.add_GET("/assets/index.css", [this](httpd_req_t* req) {
        return handle_file_(req);
    });
    server.add_GET("/assets/index.js", [this](httpd_req_t* req) {
        return handle_file_(req);
    });
}

void WebGuiPipeline::initialize_fs_() {
    esp_vfs_spiffs_conf_t fs_configuration;
    memset(&fs_configuration, 0, sizeof(fs_configuration));

    fs_configuration.base_path = mount_point_;
    fs_configuration.max_files = 5;

    auto ret = esp_vfs_spiffs_register(&fs_configuration);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ocs_loge(log_tag, "failed to mount or format filesystem: mount_point=%s",
                     mount_point_);
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ocs_loge(log_tag, "failed to find SPIFFS partition: mount_point=%s",
                     mount_point_);
        } else {
            ocs_loge(log_tag, "failed to initialize SPIFFS: mount_point=%s err=%s",
                     mount_point_, esp_err_to_name(ret));
        }

        return;
    }

    size_t total = 0;
    size_t used = 0;

    ret = esp_spiffs_info(nullptr, &total, &used);
    if (ret != ESP_OK) {
        ocs_loge(log_tag, "esp_spiffs_info(): %s", esp_err_to_name(ret));

        return;
    }

    ocs_logi(log_tag, "partition size: total=%d used=%d", total, used);

    valid_ = true;
}

status::StatusCode WebGuiPipeline::handle_file_(httpd_req_t* req) {
    if (!valid_) {
        return status::StatusCode::InvalidState;
    }

    std::string file_path = mount_point_;
    if (req->uri[strlen(req->uri) - 1] == '/') {
        file_path += "/index.html";
    } else {
        file_path += req->uri;
    }

    auto err = httpd_resp_set_type(req, parse_content_type(file_path.c_str()));
    if (err != ESP_OK) {
        ocs_loge(log_tag, "httpd_resp_set_type(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    err = httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
    if (err != ESP_OK) {
        ocs_loge(log_tag, "httpd_resp_set_hdr(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    file_path += ".gz";

    core::FileStreamReader reader(file_path.c_str());
    http::ChunkStreamWriter writer(req);
    core::StreamTransceiver transceiver(reader, writer, buffer_);

    return transceiver.transceive();
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
