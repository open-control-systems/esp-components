/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/http_client_builder.h"

namespace ocs {
namespace net {

HttpClientSharedPtr make_http_client_shared(esp_http_client_config_t& config) {
    return HttpClientSharedPtr(esp_http_client_init(&config),
                               [](esp_http_client_handle_t client) {
                                   if (client) {
                                       ESP_ERROR_CHECK(esp_http_client_close(client));
                                       ESP_ERROR_CHECK(esp_http_client_cleanup(client));
                                   }
                               });
}

} // namespace net
} // namespace ocs
