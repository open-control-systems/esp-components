/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>

namespace ocs {
namespace net {

class IApNetwork {
public:
    struct Info {
        //! WiFi channel.
        uint8_t channel { 0 };

        //! Current number of STA connections to the AP.
        uint8_t cur_connection { 0 };

        //! Maximum number of simultaneous STA connections to the AP.
        uint8_t max_connection { 0 };
    };

    //! Destroy.
    virtual ~IApNetwork() = default;

    //! Return various network characteristics.
    virtual Info get_info() = 0;
};

} // namespace net
} // namespace ocs
