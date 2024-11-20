idf_component_register(
    SRCS
    "test_sta_network.cpp"
    "test_ip_addr_to_str.cpp"
    "test_rssi_to_signal_strength.cpp"
    "test_default_mdns_driver.cpp"

    REQUIRES
    "unity"
    "ocs_net"
    "ocs_storage"
)
