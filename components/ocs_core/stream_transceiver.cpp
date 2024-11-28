/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstdio>

#include "ocs_core/log.h"
#include "ocs_core/stream_transceiver.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace core {

namespace {

const char* log_tag = "stream_transceiver";

} // namespace

StreamTransceiver::StreamTransceiver(IStreamReader& reader,
                                     IStreamWriter& writer,
                                     StreamTransceiver::Buffer& buffer)
    : reader_(reader)
    , writer_(writer)
    , buffer_(buffer) {
}

StreamTransceiver::~StreamTransceiver() {
    if (const auto code = reader_.end(); code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to end reader stream transferring: %s",
                 status::code_to_str(code));
    }

    if (const auto code = writer_.end(); code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to end writer stream transferring: %s",
                 status::code_to_str(code));
    }
}

status::StatusCode StreamTransceiver::transceive() {
    if (const auto code = reader_.begin(); code != status::StatusCode::OK) {
        return code;
    }

    if (const auto code = writer_.begin(); code != status::StatusCode::OK) {
        return code;
    }

    while (true) {
        buffer_.clear();

        unsigned size = buffer_.capacity();

        auto code = reader_.read(buffer_.data(), size);
        if (code == status::StatusCode::OK) {
            code = writer_.write(buffer_.data(), size);
        }

        if (code != status::StatusCode::OK) {
            if (code == status::StatusCode::NoData) {
                break;
            }

            return code;
        }
    }

    return status::StatusCode::OK;
}

} // namespace core
} // namespace ocs
