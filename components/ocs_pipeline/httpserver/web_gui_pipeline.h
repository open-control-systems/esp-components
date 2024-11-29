/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_core/stream_transceiver.h"
#include "ocs_http/server.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class WebGuiPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register endpoints to serve the Web GUI files.
    explicit WebGuiPipeline(http::Server& server);

private:
    void initialize_fs_();
    status::StatusCode handle_file_(httpd_req_t* req);

    static const constexpr char* mount_point_ = "/web_gui";
    static const size_t buffer_size_ = 1024;

    bool valid_ { false };
    core::StreamTransceiver::Buffer buffer_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
