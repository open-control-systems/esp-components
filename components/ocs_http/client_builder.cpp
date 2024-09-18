/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_http/client_builder.h"

namespace ocs {
namespace http {

ClientSharedPtr make_client_shared(esp_http_client_config_t& config) {
    return ClientSharedPtr(esp_http_client_init(&config),
                           [](esp_http_client_handle_t client) {
                               if (client) {
                                   ESP_ERROR_CHECK(esp_http_client_close(client));
                                   ESP_ERROR_CHECK(esp_http_client_cleanup(client));
                               }
                           });
}

} // namespace http
} // namespace ocs
