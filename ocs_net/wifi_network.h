#pragma once

#include <string>
#include <vector>

#include "esp_wifi.h"

#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_net/inetwork_handler.h"
#include "ocs_status/code.h"

namespace ocs {
namespace net {

//! Handle WiFi network operations.
class WiFiNetwork : public core::NonCopyable<> {
public:
    struct Params {
        //! Maximum number of attempts to establish a WiFi connection.
        unsigned max_retry_count { 5 };

        //! WiFi STA SSID.
        std::string ssid;

        //! WiFi STA password.
        std::string password;
    };

    //! Initialize.
    //!
    //! @remarks
    //!  - NVS should be initialized.
    explicit WiFiNetwork(const Params& params);

    //! Destroy.
    ~WiFiNetwork();

    //! Start the WiFi connection process.
    status::StatusCode start();

    //! Stop the WiFi connection process.
    status::StatusCode stop();

    //! Wait for the WiFi connection to be established.
    status::StatusCode wait();

    //! Add handler to be notified about the WiFi network connection status.
    //!
    //! @remarks
    //!  All handlers should be added before the network is started.
    void add(INetworkHandler& handler);

private:
    static void handle_event_(void* event_arg,
                              esp_event_base_t event_base,
                              int32_t event_id,
                              void* event_data);

    void handle_wifi_event_(int32_t event_id, void* event_data);
    void handle_wifi_event_sta_start_();
    void handle_wifi_event_sta_disconnected_(void* event_data);

    void handle_ip_event_(int32_t event_id, void* event_data);
    void handle_ip_event_sta_got_ip_(void* event_data);

    void handle_connected_();
    void handle_disconnected_();

    const Params params_;

    core::StaticEventGroup event_group_;

    esp_event_handler_instance_t instance_any_id_ { nullptr };
    esp_event_handler_instance_t instance_got_ip_ { nullptr };

    std::vector<INetworkHandler*> handlers_;

    unsigned retry_count_ { 0 };
};

} // namespace net
} // namespace ocs
