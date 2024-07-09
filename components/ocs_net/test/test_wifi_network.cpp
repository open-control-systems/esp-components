#include <cstring>

#include "unity.h"

#include "ocs_net/wifi_network.h"
#include "ocs_storage/flash_storage.h"

namespace ocs {
namespace net {

TEST_CASE("Connect to WiFi AP: invalid credentials", "[ocs_net], [wifi_network]") {
    { // Invalid SSID
        storage::FlashStorage storage;

        WiFiNetwork network(WiFiNetwork::Params {
            .max_retry_count = 1,
            .ssid = "foo",
            .password = CONFIG_OCS_NETWORK_WIFI_STA_PASSWORD,
        });

        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
        TEST_ASSERT_EQUAL(status::StatusCode::Error, network.wait());
        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
    }
    { // Invalid password
        storage::FlashStorage storage;

        WiFiNetwork network(WiFiNetwork::Params {
            .max_retry_count = 1,
            .ssid = CONFIG_OCS_NETWORK_WIFI_STA_SSID,
            .password = "bar",
        });

        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
        TEST_ASSERT_EQUAL(status::StatusCode::Error, network.wait());
        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
    }
    { // Invalid SSID and password
        storage::FlashStorage storage;

        WiFiNetwork network(WiFiNetwork::Params {
            .max_retry_count = 1,
            .ssid = "foo",
            .password = "bar",
        });

        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
        TEST_ASSERT_EQUAL(status::StatusCode::Error, network.wait());
        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
    }
}

#ifdef CONFIG_OCS_UNIT_TEST_NETWORK_WIFI_ENABLED
TEST_CASE("Connect to WiFi AP: valid credentials", "[ocs_net], [wifi_network]") {
    storage::FlashStorage storage;

    WiFiNetwork network(WiFiNetwork::Params {
        .max_retry_count = 3,
        .ssid = CONFIG_OCS_NETWORK_WIFI_STA_SSID,
        .password = CONFIG_OCS_NETWORK_WIFI_STA_PASSWORD,
    });

    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.wait());

    wifi_ap_record_t record;
    memset(&record, 0, sizeof(record));
    TEST_ASSERT_EQUAL(ESP_OK, esp_wifi_sta_get_ap_info(&record));
    TEST_ASSERT_EQUAL_STRING(CONFIG_OCS_NETWORK_WIFI_STA_SSID, record.ssid);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
}
#endif // CONFIG_OCS_UNIT_TEST_NETWORK_WIFI_ENABLED

} // namespace net
} // namespace ocs
