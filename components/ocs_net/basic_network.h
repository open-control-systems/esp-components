/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <optional>
#include <vector>

#include "lwip/ip_addr.h"

#include "ocs_core/noncopyable.h"
#include "ocs_net/inetwork_handler.h"
#include "ocs_status/code.h"

namespace ocs {
namespace net {

class BasicNetwork : public core::NonCopyable<BasicNetwork> {
public:
    //! Destroy.
    virtual ~BasicNetwork() = default;

    //! Start the network connection process.
    virtual status::StatusCode start() = 0;

    //! Stop the network connection process.
    virtual status::StatusCode stop() = 0;

    //! Wait for the network connection to be established.
    virtual status::StatusCode wait() = 0;

    //! Return received IP address.
    virtual std::optional<ip_addr_t> get_ip_addr() const = 0;

    //! Add handler to be notified about the network connection status.
    //!
    //! @remarks
    //!  All handlers should be added before the network is started.
    void add(INetworkHandler& handler);

protected:
    void handle_connected_();
    void handle_disconnected_();

    std::vector<INetworkHandler*> handlers_;
};

} // namespace net
} // namespace ocs
