/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_http/server.h"
#include "ocs_net/basic_network.h"
#include "ocs_net/mdns_provider.h"
#include "ocs_pipeline/httpserver/data_handler.h"
#include "ocs_pipeline/httpserver/system_handler.h"
#include "ocs_scheduler/itask.h"
#include "ocs_system/fanout_suspender.h"
#include "ocs_system/isuspend_handler.h"

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
#include "ocs_pipeline/httpserver/system_state_handler.h"
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY

namespace ocs {
namespace pipeline {
namespace httpserver {

class HttpPipeline : public net::INetworkHandler,
                     public system::ISuspendHandler,
                     public core::NonCopyable<> {
public:
    struct DataParams {
        //! Buffer size to hold the formatted JSON data, in bytes.
        unsigned buffer_size { 0 };
    };

    struct Params {
        DataParams telemetry;
        DataParams registration;
    };

    //! Initialize.
    HttpPipeline(scheduler::ITask& reboot_task,
                 system::FanoutSuspender& suspender,
                 net::BasicNetwork& network,
                 net::MdnsProvider& mdns_provider,
                 fmt::json::IFormatter& telemetry_formatter,
                 fmt::json::FanoutFormatter& registration_formatter,
                 Params params);

    //! Start HTTP server.
    void handle_connect() override;

    //! Stop HTTP server.
    void handle_disconnect() override;

    //! Disable mDNS.
    status::StatusCode handle_suspend() override;

    //! Enable mDNS.
    status::StatusCode handle_resume() override;

    //! Return HTTP server.
    http::Server& get_server();

private:
    net::MdnsProvider& mdns_provider_;

    std::unique_ptr<http::Server> http_server_;
    std::unique_ptr<DataHandler> telemetry_handler_;
    std::unique_ptr<DataHandler> registration_handler_;
    std::unique_ptr<SystemHandler> system_handler_;
    std::unique_ptr<fmt::json::IFormatter> network_formatter_;

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    std::unique_ptr<SystemStateHandler> system_state_handler_;
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
