/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <string>

#include "lwip/ip_addr.h"

namespace ocs {
namespace net {

class IStaNetwork {
public:
    struct Info {
        //! AP SSID to which STA is connected.
        std::string ssid;

        //! Signal strength of the AP.
        int8_t rssi { 0 };

        //! Assigned IP address.
        ip_addr_t ip_addr = IPADDR4_INIT(0);
    };

    //! Destroy.
    virtual ~IStaNetwork() = default;

    //! Return various network characteristics.
    virtual Info get_info() = 0;
};

} // namespace net
} // namespace ocs
