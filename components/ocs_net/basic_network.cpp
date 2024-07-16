#include "ocs_net/basic_network.h"

namespace ocs {
namespace net {

void BasicNetwork::add(INetworkHandler& handler) {
    handlers_.emplace_back(&handler);
}

void BasicNetwork::handle_connected_() {
    for (auto& handler : handlers_) {
        handler->handle_connected();
    }
}

void BasicNetwork::handle_disconnected_() {
    for (auto& handler : handlers_) {
        handler->handle_disconnected();
    }
}

} // namespace net
} // namespace ocs
