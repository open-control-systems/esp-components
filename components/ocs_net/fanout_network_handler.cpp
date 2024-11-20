/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/fanout_network_handler.h"

namespace ocs {
namespace net {

void FanoutNetworkHandler::handle_connect() {
    for (auto& handler : handlers_) {
        handler->handle_connect();
    }
}

void FanoutNetworkHandler::handle_disconnect() {
    for (auto& handler : handlers_) {
        handler->handle_disconnect();
    }
}

void FanoutNetworkHandler::add(INetworkHandler& handler) {
    handlers_.push_back(&handler);
}

} // namespace net
} // namespace ocs
