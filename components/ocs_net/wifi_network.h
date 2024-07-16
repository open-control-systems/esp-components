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

#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_net/basic_network.h"
#include "ocs_net/netif_builder.h"
#include "ocs_status/code.h"

namespace ocs {
namespace net {

//! Handle WiFi network operations.
class WiFiNetwork : public BasicNetwork, public core::NonCopyable<> {
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
    status::StatusCode start() override;

    //! Stop the WiFi connection process.
    status::StatusCode stop() override;

    //! Wait for the WiFi connection to be established.
    status::StatusCode wait() override;

    //! Return received IP address.
    std::optional<ip_addr_t> get_ip_addr() const override;

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

    const Params params_;

    NetifSharedPtr netif_;

    core::StaticEventGroup event_group_;

    esp_event_handler_instance_t instance_any_id_ { nullptr };
    esp_event_handler_instance_t instance_got_ip_ { nullptr };

    unsigned retry_count_ { 0 };
};

} // namespace net
} // namespace ocs
