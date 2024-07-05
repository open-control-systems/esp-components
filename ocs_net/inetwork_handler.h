#pragma once

namespace ocs {
namespace net {

class INetworkHandler {
public:
    //! Destroy.
    virtual ~INetworkHandler() = default;

    //! Network is connected.
    virtual void handle_connected() = 0;

    //! Network is disconnected.
    virtual void handle_disconnected() = 0;
};

} // namespace net
} // namespace ocs
