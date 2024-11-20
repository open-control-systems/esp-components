/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>

#include "esp_wifi.h"

#include "ocs_core/cond.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_core/static_mutex.h"
#include "ocs_net/basic_network.h"
#include "ocs_net/inetwork_handler.h"
#include "ocs_net/netif_builder.h"
#include "ocs_status/code.h"

namespace ocs {
namespace net {

//! Handle WiFi AP (access-point) mode network operations.
class ApNetwork : public BasicNetwork, public core::NonCopyable<> {
public:
    struct Params {
        //! WiFi SSID.
        std::string ssid;

        //! WiFi password.
        std::string password;

        //! WiFi channel.
        int channel { 0 };

        //! Maximum number of simultaneous STA connection to the AP.
        int max_connection { 0 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p handler to notify about network status changes.
    ApNetwork(INetworkHandler& handler, const Params& params);

    //! Destroy.
    ~ApNetwork();

    //! Start the WiFi connection process.
    status::StatusCode start() override;

    //! Stop the WiFi connection process.
    status::StatusCode stop() override;

    //! Wait for the WiFi connection to be established.
    status::StatusCode wait(TickType_t wait = portMAX_DELAY) override;

    //! Return received IP address.
    std::optional<ip_addr_t> get_ip_addr() const override;

private:
    static void handle_event_(void* event_arg,
                              esp_event_base_t event_base,
                              int32_t event_id,
                              void* event_data);

    void handle_wifi_event_(int32_t event_id, void* event_data);
    void handle_wifi_event_ap_sta_start_();
    void handle_wifi_event_ap_sta_stop_();
    void handle_wifi_event_ap_sta_connected_(void* event_data);
    void handle_wifi_event_ap_sta_disconnected_(void* event_data);

    const Params params_;

    INetworkHandler& handler_;

    NetifSharedPtr netif_;

    esp_event_handler_instance_t instance_any_id_ { nullptr };

    core::StaticMutex mu_;
    core::Cond cond_;
    status::StatusCode code_ { status::StatusCode::Last };
};

} // namespace net
} // namespace ocs
