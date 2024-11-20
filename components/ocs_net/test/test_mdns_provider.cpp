/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_net/ap_network.h"
#include "ocs_net/fanout_network_handler.h"
#include "ocs_net/mdns_provider.h"
#include "ocs_storage/flash_initializer.h"

namespace ocs {
namespace net {

TEST_CASE("mDNS start/stop", "[ocs_net], [mdns_provider]") {
    storage::FlashInitializer flash_initializer;
    FanoutNetworkHandler handler;

    ApNetwork network(handler,
                      ApNetwork::Params {
                          .ssid = "test-ssid",
                          .password = "test-password",
                      });

    MdnsProvider provider(MdnsProvider::Params {
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
