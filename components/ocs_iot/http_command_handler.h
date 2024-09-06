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
#include "ocs_scheduler/itask.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace iot {

class HttpCommandHandler : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register HTTP commands.
    //!  - @p reboot_task to initiate the reboot process.
    //!  - @p buffer_size to hold the formatted JSON data, in bytes.
    HttpCommandHandler(net::HttpServer& server,
                       scheduler::ITask& reboot_task,
                       unsigned buffer_size);

private:
    void format_commands_response_();

    void register_routes_(net::HttpServer& server, scheduler::ITask& reboot_task);

    std::unique_ptr<DynamicJsonFormatter> commands_response_;
};

} // namespace iot
} // namespace ocs
