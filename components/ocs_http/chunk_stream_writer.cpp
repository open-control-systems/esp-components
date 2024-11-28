/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_http/chunk_stream_writer.h"

namespace ocs {
namespace http {

namespace {

const char* log_tag = "chunk_stream_writer";

} // namespace

ChunkStreamWriter::ChunkStreamWriter(httpd_req_t* req)
    : req_(req) {
    configASSERT(req_);
}

status::StatusCode ChunkStreamWriter::begin() {
    return status::StatusCode::OK;
}

status::StatusCode ChunkStreamWriter::end() {
    // Respond with an empty chunk to signal HTTP response completion.
    return send_(nullptr, 0);
}

status::StatusCode ChunkStreamWriter::cancel() {
    return end();
}

status::StatusCode ChunkStreamWriter::write(const void* data, unsigned size) {
    return send_(data, size);
}

status::StatusCode ChunkStreamWriter::send_(const void* data, unsigned size) {
    const auto err = httpd_resp_send_chunk(req_, static_cast<const char*>(data), size);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "httpd_resp_send_chunk(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace http
} // namespace ocs
