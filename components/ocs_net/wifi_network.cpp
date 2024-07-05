#include <cstring>

#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"

#include "ocs_net/wifi_network.h"

#define EVENT_BIT_CONNECTED BIT0
#define EVENT_BIT_FAILED BIT1

namespace ocs {
namespace net {

namespace {

const char* log_tag = "wifi-network";

} // namespace

WiFiNetwork::WiFiNetwork(const Params& params)
    : params_(params) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &handle_event_, this, &instance_any_id_));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &handle_event_, this, &instance_got_ip_));

    wifi_config_t wifi_config;
    memset(&wifi_config, 0, sizeof(wifi_config));

    strncpy(reinterpret_cast<char*>(wifi_config.sta.ssid), params_.ssid.c_str(),
            sizeof(wifi_config.sta.ssid));
    wifi_config.sta.ssid[sizeof(wifi_config.sta.ssid) - 1] = 0;

    strncpy(reinterpret_cast<char*>(wifi_config.sta.password), params_.password.c_str(),
            sizeof(wifi_config.sta.password));
    wifi_config.sta.password[sizeof(wifi_config.sta.password) - 1] = 0;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
}

WiFiNetwork::~WiFiNetwork() {
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                                          &instance_any_id_));

    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                                          &instance_got_ip_));
}

status::StatusCode WiFiNetwork::start() {
    const auto err = esp_wifi_start();
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "esp_wifi_start(): %s", esp_err_to_name(err));
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode WiFiNetwork::stop() {
    const auto err = esp_wifi_stop();
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "esp_wifi_stop(): %s", esp_err_to_name(err));
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode WiFiNetwork::wait() {
    const EventBits_t bits =
        xEventGroupWaitBits(event_group_.get(), EVENT_BIT_CONNECTED | EVENT_BIT_FAILED,
                            pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & EVENT_BIT_CONNECTED) {
        ESP_LOGI(log_tag, "connected to AP: SSID:%s", CONFIG_OCS_NETWORK_WIFI_STA_SSID);
        return status::StatusCode::OK;
    }

    if (bits & EVENT_BIT_FAILED) {
        ESP_LOGI(log_tag, "failed to connect to AP: SSID:%s",
                 CONFIG_OCS_NETWORK_WIFI_STA_SSID);
        return status::StatusCode::Error;
    }

    ESP_LOGE(log_tag, "unexpected event bit: bits=%ld", bits);
    configASSERT(false);
}

void WiFiNetwork::add(INetworkHandler& handler) {
    handlers_.emplace_back(&handler);
}

void WiFiNetwork::handle_event_(void* event_arg,
                                esp_event_base_t event_base,
                                int32_t event_id,
                                void* event_data) {
    configASSERT(event_base == WIFI_EVENT || event_base == IP_EVENT);
    configASSERT(event_arg);

    WiFiNetwork& self = *static_cast<WiFiNetwork*>(event_arg);

    if (event_base == WIFI_EVENT) {
        self.handle_wifi_event_(event_id, event_data);
    } else if (event_base == IP_EVENT) {
        self.handle_ip_event_(event_id, event_data);
    } else {
        ESP_LOGE(log_tag, "unsupported event: event_id=%ld", event_id);
    }
}

void WiFiNetwork::handle_wifi_event_(int32_t event_id, void* event_data) {
    switch (event_id) {
    case WIFI_EVENT_STA_START:
        handle_wifi_event_sta_start_();
        break;

    case WIFI_EVENT_STA_DISCONNECTED:
        handle_wifi_event_sta_disconnected_(event_data);
        break;

    default:
        break;
    }
}

void WiFiNetwork::handle_wifi_event_sta_start_() {
    ESP_LOGI(log_tag, "WIFI_EVENT_STA_START");

    const esp_err_t err = esp_wifi_connect();
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "esp_wifi_connect(): %s", esp_err_to_name(err));
    }
}

void WiFiNetwork::handle_wifi_event_sta_disconnected_(void* event_data) {
    configASSERT(event_data);

    wifi_event_sta_disconnected_t& data =
        *static_cast<wifi_event_sta_disconnected_t*>(event_data);

    ESP_LOGI(log_tag, "WIFI_EVENT_STA_DISCONNECTED: reason=%u", data.reason);

    xEventGroupClearBits(event_group_.get(), EVENT_BIT_CONNECTED);

    if (retry_count_ < params_.max_retry_count) {
        ++retry_count_;

        const esp_err_t err = esp_wifi_connect();
        if (err != ESP_OK) {
            ESP_LOGE(log_tag, "esp_wifi_connect(): %s", esp_err_to_name(err));
        } else {
            ESP_LOGI(log_tag, "reconnecting: retry_count=%u max_retry_count=%u",
                     retry_count_, params_.max_retry_count);
        }
    } else {
        xEventGroupSetBits(event_group_.get(), EVENT_BIT_FAILED);
    }

    handle_disconnected_();
}

void WiFiNetwork::handle_ip_event_(int32_t event_id, void* event_data) {
    switch (event_id) {
    case IP_EVENT_STA_GOT_IP:
        handle_ip_event_sta_got_ip_(event_data);
        break;

    default:
        break;
    }
}

void WiFiNetwork::handle_ip_event_sta_got_ip_(void* event_data) {
    ip_event_got_ip_t& event = *static_cast<ip_event_got_ip_t*>(event_data);
    ESP_LOGI(log_tag, "got ip:" IPSTR "", IP2STR(&event.ip_info.ip));

    retry_count_ = 0;
    xEventGroupSetBits(event_group_.get(), EVENT_BIT_CONNECTED);

    handle_connected_();
}

void WiFiNetwork::handle_connected_() {
    for (auto& handler : handlers_) {
        handler->handle_connected();
    }
}

void WiFiNetwork::handle_disconnected_() {
    for (auto& handler : handlers_) {
        handler->handle_disconnected();
    }
}

} // namespace net
} // namespace ocs
