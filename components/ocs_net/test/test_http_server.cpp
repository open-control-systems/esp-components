/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <cstring>

#include "esp_log.h"
#include "unity.h"

#include "ocs_net/http_client_reader.h"
#include "ocs_net/http_server.h"
#include "ocs_net/ip_addr_to_str.h"
#include "ocs_net/wifi_network.h"
#include "ocs_storage/flash_initializer.h"

namespace ocs {
namespace net {

TEST_CASE("Stop HTTP server: no start", "[ocs_net], [http_server]") {
    HTTPServer server(HTTPServer::Params {});
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());
}

TEST_CASE("Start HTTP server: no WiFi", "[ocs_net], [http_server]") {
    HTTPServer server(HTTPServer::Params {});
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());
}

TEST_CASE("Start HTTP server: WiFi invalid credentials", "[ocs_net], [http_server]") {
    storage::FlashInitializer flash_initializer;

    WiFiNetwork wifi_network(WiFiNetwork::Params {
        .max_retry_count = 1,
        .ssid = "foo",
        .password = "bar",
    });
    TEST_ASSERT_EQUAL(status::StatusCode::OK, wifi_network.start());
    TEST_ASSERT_EQUAL(status::StatusCode::Error, wifi_network.wait());

    HTTPServer server(HTTPServer::Params {});
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, wifi_network.stop());
}

#ifdef CONFIG_OCS_UNIT_TEST_NETWORK_WIFI_ENABLED
TEST_CASE("Start HTTP server: WiFi valid credentials", "[ocs_net], [http_server]") {
    const unsigned server_port = 80;

    HTTPServer server(HTTPServer::Params {
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

    WiFiNetwork wifi_network(WiFiNetwork::Params {
        .max_retry_count = 1,
        .ssid = CONFIG_OCS_NETWORK_WIFI_STA_SSID,
        .password = CONFIG_OCS_NETWORK_WIFI_STA_PASSWORD,
    });
    TEST_ASSERT_EQUAL(status::StatusCode::OK, wifi_network.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, wifi_network.wait());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.start());

    const auto ip_addr = wifi_network.get_ip_addr();
    TEST_ASSERT_TRUE(ip_addr);

    ip_addr_to_str ip_addr_str(*ip_addr);

    HTTPClientReader reader(HTTPClientReader::Params {
        .host = ip_addr_str.c_str(),
        .path = path,
        .bufsize = 128,
    });
    TEST_ASSERT_EQUAL(ESP_OK, esp_http_client_perform(reader.client()));
    TEST_ASSERT_EQUAL(200, esp_http_client_get_status_code(reader.client()));

    TEST_ASSERT_TRUE(reader.wait());

    char got_response[strlen(want_response) + 1];
    memset(got_response, 0, sizeof(got_response));
    TEST_ASSERT_EQUAL(strlen(want_response),
                      reader.read(got_response, strlen(want_response)));

    TEST_ASSERT_EQUAL_STRING(want_response, got_response);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, wifi_network.stop());
}
#endif // CONFIG_OCS_UNIT_TEST_NETWORK_WIFI_ENABLED

} // namespace net
} // namespace ocs
