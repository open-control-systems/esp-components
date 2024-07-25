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
#include "ocs_net/basic_network.h"
#include "ocs_net/http_server.h"
#include "ocs_net/inetwork_handler.h"
#include "ocs_net/mdns_provider.h"

namespace ocs {
namespace iot {

class HTTPServerPipeline : public net::INetworkHandler, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @remarks
    //!  NVS should be initialized.
    HTTPServerPipeline();

    //! Start HTTP server.
    void handle_connected() override;

    //! Stop HTTP server.
    void handle_disconnected() override;

    //! Start network services.
    status::StatusCode start();

    //! Return network instance.
    net::BasicNetwork& network();

    //! Return HTTP server.
    net::HTTPServer& server();

    //! Return mDNS instance.
    net::MDNSProvider& mdns();

private:
    status::StatusCode try_start_wifi_();
    void stop_wifi_();

    status::StatusCode try_start_mdns_();
    void stop_mdns_();

    std::unique_ptr<net::BasicNetwork> wifi_network_;
    std::unique_ptr<net::HTTPServer> http_server_;
    std::unique_ptr<net::MDNSProvider> mdns_provider_;
};

} // namespace iot
} // namespace ocs
