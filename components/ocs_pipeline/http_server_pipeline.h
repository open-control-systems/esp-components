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
#include "ocs_http/server.h"
#include "ocs_net/basic_network.h"
#include "ocs_net/inetwork_handler.h"
#include "ocs_net/mdns_provider.h"
#include "ocs_system/isuspend_handler.h"

namespace ocs {
namespace pipeline {

class HttpServerPipeline : public net::INetworkHandler,
                           public system::ISuspendHandler,
                           public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @remarks
    //!  NVS should be initialized.
    HttpServerPipeline();

    //! Start HTTP server.
    void handle_connect() override;

    //! Stop HTTP server.
    void handle_disconnect() override;

    //! Disable mDNS.
    status::StatusCode handle_suspend() override;

    //! Enable mDNS.
    status::StatusCode handle_resume() override;

    //! Start network services.
    status::StatusCode start();

    //! Return network instance.
    net::BasicNetwork& network();

    //! Return HTTP server.
    http::Server& server();

    //! Return mDNS instance.
    net::MdnsProvider& mdns();

private:
    status::StatusCode try_start_wifi_();
    void stop_wifi_();

    status::StatusCode try_start_mdns_();

    std::unique_ptr<net::BasicNetwork> wifi_network_;
    std::unique_ptr<http::Server> http_server_;
    std::unique_ptr<net::MdnsProvider> mdns_provider_;
};

} // namespace pipeline
} // namespace ocs
