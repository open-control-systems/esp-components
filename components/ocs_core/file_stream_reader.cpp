/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cerrno>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/file_stream_reader.h"
#include "ocs_core/log.h"

namespace ocs {
namespace core {

namespace {

const char* log_tag = "file_stream_reader";

} // namespace

FileStreamReader::FileStreamReader(const char* path)
    : path_(path) {
    configASSERT(path_.size());
}

FileStreamReader::~FileStreamReader() {
    configASSERT(!file_);
}

status::StatusCode FileStreamReader::begin() {
    if (!file_) {
        file_ = fopen(path_.c_str(), "r");
    }
    if (!file_) {
        ocs_loge(log_tag, "failed to open file: path=%s err=%s", path_.c_str(),
                 std::strerror(errno));
    }

    return file_ ? status::StatusCode::OK : status::StatusCode::Error;
}

status::StatusCode FileStreamReader::end() {
    int ret = 0;

    if (file_) {
        ret = fclose(file_);
    }
    if (ret) {
        ocs_loge(log_tag, "failed to close file: path=%s err=%s", path_.c_str(),
                 std::strerror(errno));
    }

    file_ = nullptr;

    return ret ? status::StatusCode::Error : status::StatusCode::OK;
}

status::StatusCode FileStreamReader::cancel() {
    int ret = 0;

    if (file_) {
        ret = fseek(file_, 0L, SEEK_SET);
    }
    if (ret) {
        ocs_loge(log_tag, "failed to rewind the file: path=%s err=%s", path_.c_str(),
                 std::strerror(errno));
    }

    return ret ? status::StatusCode::Error : status::StatusCode::OK;
}

status::StatusCode FileStreamReader::read(void* data, unsigned& size) {
    configASSERT(file_);

    if (feof(file_)) {
        return status::StatusCode::NoData;
    }

    const auto read_bytes = fread(data, 1, size, file_);
    if (read_bytes < size && ferror(file_)) {
        ocs_loge(log_tag, "failed to read from file: path=%s err=%s", path_.c_str(),
                 std::strerror(errno));

        return status::StatusCode::Error;
    }

    size = read_bytes;

    return status::StatusCode::OK;
}

} // namespace core
} // namespace ocs
