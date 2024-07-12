/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "esp_netif.h"

namespace ocs {
namespace net {

using NetifSharedPtr = std::shared_ptr<esp_netif_t>;

NetifSharedPtr make_netif_shared(esp_netif_t* netif);

} // namespace net
} // namespace ocs
