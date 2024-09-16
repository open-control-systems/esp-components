idf_component_register(
    SRCS
    "ds18b20/test_store.cpp"
    "ds18b20/test_parse_configuration.cpp"

    REQUIRES
    "unity"
    "ocs_sensor"
)
