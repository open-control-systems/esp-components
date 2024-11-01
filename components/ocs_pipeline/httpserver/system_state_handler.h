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
#include "ocs_http/server.h"
#include "ocs_net/mdns_provider.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class SystemStateHandler : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register endpoint to receive system statistics.
    //!  - @p provider to register mDNS txt records.
    //!  - @p response_size - system state response size, in bytes.
    SystemStateHandler(http::Server& server,
                       net::MdnsProvider& provider,
                       unsigned response_size);

private:
    std::unique_ptr<fmt::json::IFormatter> state_json_formatter_;
    std::unique_ptr<fmt::json::DynamicFormatter> json_formatter_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
