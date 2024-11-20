/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_net/ap_network.h"
#include "ocs_net/default_mdns_driver.h"
#include "ocs_net/fanout_network_handler.h"
#include "ocs_storage/flash_initializer.h"

namespace ocs {
namespace net {

TEST_CASE("Default mDNS driver: start/stop", "[ocs_net], [default_mdns_driver]") {
    storage::FlashInitializer flash_initializer;
    FanoutNetworkHandler handler;

    ApNetwork network(handler,
                      ApNetwork::Params {
                          .ssid = "test-ssid",
                          .password = "test-password",
                      });

    DefaultMdnsDriver mdns_driver(DefaultMdnsDriver::Params {
        .hostname = "host",
        .instance_name = "instance",
    });

    TEST_ASSERT_EQUAL(status::StatusCode::OK, mdns_driver.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, mdns_driver.stop());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, mdns_driver.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, mdns_driver.stop());
}

} // namespace net
} // namespace ocs
