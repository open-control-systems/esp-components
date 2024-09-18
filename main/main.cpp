/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstdio>

#include "ocs_net/mdns_provider.h"
#include "ocs_net/wifi_network.h"
#include "ocs_status/code.h"
#include "ocs_status/code_to_str.h"
#include "ocs_storage/flash_storage.h"

namespace {

const char* log_tag = "app-main";

} // namespace

using namespace ocs;

extern "C" void app_main(void) {
    storage::FlashStorage storage;

    net::WiFiNetwork wifi_network(net::WiFiNetwork::Params {
        .max_retry_count = 32,
        .ssid = "foo",
        .password = "password",
    });

    net::MDNSProvider mdns_provider(net::MDNSProvider::Params {
        .hostname = "foo",
        .instance_name = "bar",
    });
    const auto status = mdns_provider.add_service("foo", "bar", 80);
    ocs_logi(log_tag, "status=%s", status::code_to_str(status));

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
