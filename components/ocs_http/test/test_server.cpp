/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <cstring>
#include <string>

#include "unity.h"

#include "ocs_http/client_reader.h"
#include "ocs_http/server.h"
#include "ocs_net/fanout_network_handler.h"
#include "ocs_net/ip_addr_to_str.h"
#include "ocs_net/sta_network.h"
#include "ocs_storage/flash_initializer.h"

#ifdef CONFIG_OCS_TEST_UNIT_WIFI_STA_ENABLED
#include "ocs_core/istream_reader.h"
#include "ocs_core/stream_transceiver.h"
#include "ocs_http/chunk_stream_writer.h"
#endif // CONFIG_OCS_TEST_UNIT_WIFI_STA_ENABLED

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
    const char* want_response = "hello world";

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

TEST_CASE("Start HTTP server: chunked response", "[ocs_http], [server]") {
    struct TestStreamReader : public core::IStreamReader {
        TestStreamReader(const char* response, unsigned chunk_size)
            : response_(response)
            , chunk_size_(chunk_size) {
        }

        status::StatusCode begin() override {
            return status::StatusCode::OK;
        }

        status::StatusCode end() override {
            return status::StatusCode::OK;
        }

        status::StatusCode cancel() override {
            return status::StatusCode::OK;
        }

        status::StatusCode read(void* data, unsigned& size) override {
            if (pos_ == response_.size()) {
                return status::StatusCode::NoData;
            }

            const auto remain = response_.size() - pos_;
            size = std::min(remain, chunk_size_);

            memcpy(data, response_.data() + pos_, size);
            pos_ += size;

            return status::StatusCode::OK;
        }

    private:
        const std::string response_;
        const unsigned chunk_size_ { 0 };

        unsigned pos_ { 0 };
    };

    const unsigned server_port = 80;

    Server server(Server::Params {
        .server_port = server_port,
    });

    const char* path = "/foo";
    const char* want_response = "hello world";

    server.add_GET(path, [want_response](httpd_req_t* req) {
        core::StreamTransceiver::Buffer buffer;
        buffer.resize(strlen(want_response));

        ChunkStreamWriter writer(req);
        TestStreamReader reader(want_response, 1);

        core::StreamTransceiver transceiver(reader, writer, buffer);
        return transceiver.transceive();
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
