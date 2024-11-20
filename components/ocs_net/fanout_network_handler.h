/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_net/inetwork_handler.h"

namespace ocs {
namespace net {

class FanoutNetworkHandler : public INetworkHandler, public core::NonCopyable<> {
public:
    //! Notify subscribers about network connection.
    void handle_connect() override;

    //! Notify subscribers about network disconnection.
    void handle_disconnect() override;

    //! Add @p handler to be notified when the network status is changed.
    //!
    //! @remarks
    //!  All handlers should be added before the network is started.
    void add(INetworkHandler& handler);

private:
    std::vector<INetworkHandler*> handlers_;
};

} // namespace net
} // namespace ocs
