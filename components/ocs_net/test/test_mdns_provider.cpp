/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_net/mdns_provider.h"
#include "ocs_net/wifi_network.h"
#include "ocs_storage/flash_initializer.h"

namespace ocs {
namespace net {

TEST_CASE("mDNS start/stop", "[ocs_net], [mdns_provider]") {
    storage::FlashInitializer flash_initializer;

    WiFiNetwork network(WiFiNetwork::Params {
        .max_retry_count = 3,
        .ssid = CONFIG_OCS_NETWORK_WIFI_STA_SSID,
        .password = CONFIG_OCS_NETWORK_WIFI_STA_PASSWORD,
    });

    MDNSProvider provider(MDNSProvider::Params {
        .hostname = "host",
        .instance_name = "instance",
    });

    TEST_ASSERT_EQUAL(status::StatusCode::OK, provider.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, provider.stop());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, provider.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, provider.stop());
}

} // namespace net
} // namespace ocs
