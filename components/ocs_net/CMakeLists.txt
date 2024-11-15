idf_component_register(
    SRCS
    "sta_network.cpp"
    "mdns_provider.cpp"
    "netif_builder.cpp"
    "ip_addr_to_str.cpp"
    "basic_network.cpp"
    "rssi_to_signal_strength.cpp"

    REQUIRES
    "esp_netif"
    "esp_wifi"
    "mdns"
    "ocs_core"

    INCLUDE_DIRS
    ".."
)
