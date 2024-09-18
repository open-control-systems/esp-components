/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "esp_http_client.h"

namespace ocs {
namespace http {

using ClientSharedPtr =
    std::shared_ptr<std::remove_pointer<esp_http_client_handle_t>::type>;

ClientSharedPtr make_client_shared(esp_http_client_config_t& config);

} // namespace http
} // namespace ocs
