/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/basic_network.h"

namespace ocs {
namespace net {

void BasicNetwork::add(INetworkHandler& handler) {
    handlers_.emplace_back(&handler);
}

void BasicNetwork::handle_connect_() {
    for (auto& handler : handlers_) {
        handler->handle_connect();
    }
}

void BasicNetwork::handle_disconnect_() {
    for (auto& handler : handlers_) {
        handler->handle_disconnect();
    }
}

} // namespace net
} // namespace ocs
