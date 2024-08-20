/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_iot/dynamic_json_formatter.h"
#include "ocs_iot/fanout_json_formatter.h"
#include "ocs_net/http_server.h"

namespace ocs {
namespace iot {

class HttpDataHandler : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register the HTTP endpoint.
    //!  - @p formatter to format the data.
    //!  - @p path - URI path.
    //!  - @p log_tag - ESP-IDF log tag.
    //!  - @p buffer_size to hold the formatted JSON data, in bytes.
    HttpDataHandler(net::HttpServer& server,
                    IJsonFormatter& formatter,
                    const char* path,
                    const char* log_tag,
                    unsigned buffer_size);

private:
    std::unique_ptr<FanoutJsonFormatter> fanout_formatter_;
    std::unique_ptr<DynamicJsonFormatter> json_formatter_;
};

} // namespace iot
} // namespace ocs
