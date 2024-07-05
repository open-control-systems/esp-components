idf_component_register(
    SRCS
    "wifi_network.cpp"
    "mdns_provider.cpp"

    REQUIRES
    "esp_netif"
    "esp_wifi"
    "mdns"
    "ocs_core"

    INCLUDE_DIRS
    ".."
)
