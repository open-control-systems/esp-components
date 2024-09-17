/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_net/http_server.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace iot {

class HttpSystemHandler : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register HTTP endpoints.
    //!  - @p reboot_task to initiate the reboot process.
    HttpSystemHandler(net::HttpServer& server, scheduler::ITask& reboot_task);
};

} // namespace iot
} // namespace ocs
