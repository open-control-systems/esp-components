idf_component_register(
    SRCS
    "sta_network.cpp"
    "ap_network.cpp"
    "mdns_store.cpp"
    "default_mdns_driver.cpp"
    "noop_mdns_driver.cpp"
    "mdns_to_str.cpp"
    "mdns_pipeline.cpp"
    "netif_builder.cpp"
    "ip_addr_to_str.cpp"
    "rssi_to_signal_strength.cpp"
    "fanout_network_handler.cpp"

    REQUIRES
    "esp_netif"
    "esp_wifi"
    "mdns"
    "ocs_core"

    INCLUDE_DIRS
    ".."
)
