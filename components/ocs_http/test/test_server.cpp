/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <cstring>

#include "unity.h"

#include "ocs_http/client_reader.h"
#include "ocs_http/server.h"
#include "ocs_net/fanout_network_handler.h"
#include "ocs_net/ip_addr_to_str.h"
#include "ocs_net/sta_network.h"
#include "ocs_storage/flash_initializer.h"

namespace ocs {
namespace http {

TEST_CASE("Stop HTTP server: no start", "[ocs_http], [server]") {
    Server server(Server::Params {});
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());
}

TEST_CASE("Start HTTP server: WiFi not started", "[ocs_http], [server]") {
    storage::FlashInitializer flash_initializer;
    net::FanoutNetworkHandler handler;

    net::StaNetwork network(handler,
                            net::StaNetwork::Params {
                                .max_retry_count = 1,
                                .ssid = "foo",
                                .password = "bar",
                            });

    Server server(Server::Params {});
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());
}

TEST_CASE("Start HTTP server: WiFi invalid credentials", "[ocs_http], [server]") {
    storage::FlashInitializer flash_initializer;
    net::FanoutNetworkHandler handler;

    net::StaNetwork network(handler,
                            net::StaNetwork::Params {
                                .max_retry_count = 1,
                                .ssid = "foo",
                                .password = "bar",
                            });
    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
    TEST_ASSERT_EQUAL(status::StatusCode::Error, network.wait());

    Server server(Server::Params {});
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
}

#ifdef CONFIG_OCS_TEST_UNIT_WIFI_STA_ENABLED
TEST_CASE("Start HTTP server: WiFi valid credentials", "[ocs_http], [server]") {
    const unsigned server_port = 80;

    Server server(Server::Params {
        .server_port = server_port,
    });

    const char* path = "/foo";
    const char* want_response = "hellow world";

    server.add_GET(path, [want_response](httpd_req_t* req) {
        const auto err = httpd_resp_send(req, want_response, HTTPD_RESP_USE_STRLEN);
        if (err != ESP_OK) {
            return status::StatusCode::Error;
        }

        return status::StatusCode::OK;
    });

    storage::FlashInitializer flash_initializer;
    net::FanoutNetworkHandler handler;

    net::StaNetwork network(handler,
                            net::StaNetwork::Params {
                                .max_retry_count = 1,
                                .ssid = CONFIG_OCS_TEST_UNIT_WIFI_STA_SSID,
                                .password = CONFIG_OCS_TEST_UNIT_WIFI_STA_PASSWORD,
                            });
    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.wait());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.start());

    const auto info = network.get_info();

    net::ip_addr_to_str ip_addr_str(info.ip_addr);

    ClientReader reader(ClientReader::Params {
        .host = ip_addr_str.c_str(),
        .path = path,
        .bufsize = 128,
    });
    TEST_ASSERT_EQUAL(ESP_OK, esp_http_client_perform(reader.client()));
    TEST_ASSERT_EQUAL(200, esp_http_client_get_status_code(reader.client()));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, reader.wait());

    char got_response[strlen(want_response) + 1];
    memset(got_response, 0, sizeof(got_response));
    TEST_ASSERT_EQUAL(strlen(want_response),
                      reader.read(got_response, strlen(want_response)));

    TEST_ASSERT_EQUAL_STRING(want_response, got_response);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
}
#endif // CONFIG_OCS_TEST_UNIT_WIFI_STA_ENABLED

} // namespace http
} // namespace ocs
