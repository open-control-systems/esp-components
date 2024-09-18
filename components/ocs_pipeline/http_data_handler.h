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
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_net/http_server.h"
#include "ocs_net/mdns_provider.h"

namespace ocs {
namespace pipeline {

class HttpDataHandler : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register the HTTP endpoint.
    //!  - @p provider to register mDNS txt records.
    //!  - @p formatter to format the data.
    //!  - @p path - URI path.
    //!  - @p id - unique data ID, to distinguish one data from another.
    //!  - @p buffer_size to hold the formatted JSON data, in bytes.
    HttpDataHandler(net::HttpServer& server,
                    net::MdnsProvider& provider,
                    fmt::json::IFormatter& formatter,
                    const char* path,
                    const char* id,
                    unsigned buffer_size);

private:
    std::unique_ptr<fmt::json::FanoutFormatter> fanout_formatter_;
    std::unique_ptr<fmt::json::DynamicFormatter> json_formatter_;
};

} // namespace pipeline
} // namespace ocs
