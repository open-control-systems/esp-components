idf_component_register(
    SRCS
    "wifi_network.cpp"
    "mdns_provider.cpp"
    "http_server.cpp"
    "netif_builder.cpp"

    REQUIRES
    "esp_netif"
    "esp_wifi"
    "mdns"
    "esp_http_server"
    "ocs_core"

    INCLUDE_DIRS
    ".."
)
