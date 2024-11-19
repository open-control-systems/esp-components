/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_net/basic_network.h"

namespace ocs {
namespace pipeline {
namespace network {

//! Select the WiFi network.
class INetworkSelector {
public:
    //! Destroy.
    virtual ~INetworkSelector() = default;

    //! Return the selected WiFi network.
    virtual net::BasicNetwork& get_network() = 0;
};

} // namespace network
} // namespace pipeline
} // namespace ocs
