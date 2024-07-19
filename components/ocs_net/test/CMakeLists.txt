idf_component_register(
    SRCS
    "test_wifi_network.cpp"
    "test_http_server.cpp"
    "test_ip_addr_to_str.cpp"
    "test_rssi_to_str.cpp"

    REQUIRES
    "unity"
    "ocs_net"
    "ocs_storage"
)
