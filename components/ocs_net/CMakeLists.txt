idf_component_register(
    SRCS
    "wifi_network.cpp"
    "mdns_provider.cpp"
    "http_server.cpp"
    "netif_builder.cpp"
    "http_client_builder.cpp"
    "http_client_reader.cpp"
    "ip_addr_to_str.cpp"
    "basic_network.cpp"
    "rssi_to_signal_strength.cpp"

    REQUIRES
    "esp_netif"
    "esp_wifi"
    "mdns"
    "esp_http_server"
    "esp_http_client"
    "ocs_core"

    INCLUDE_DIRS
    ".."
)
