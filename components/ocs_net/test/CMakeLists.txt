idf_component_register(
    SRCS
    "test_wifi_network.cpp"
    "test_ip_addr_to_str.cpp"
    "test_rssi_to_signal_strength.cpp"
    "test_mdns_provider.cpp"

    REQUIRES
    "unity"
    "ocs_net"
    "ocs_storage"
)
