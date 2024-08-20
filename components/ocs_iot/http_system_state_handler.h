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
#include "ocs_net/http_server.h"

namespace ocs {
namespace iot {

class HttpSystemStateHandler : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register endpoint to receive system statistics.
    //!  - @p response_size - system state response size, in bytes.
    HttpSystemStateHandler(net::HttpServer& server, unsigned response_size);

private:
    std::unique_ptr<IJsonFormatter> state_json_formatter_;
    std::unique_ptr<DynamicJsonFormatter> json_formatter_;
};

} // namespace iot
} // namespace ocs
