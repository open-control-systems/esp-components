idf_component_register(
    SRCS
    "gpio/test_gpio_guard.cpp"

    "adc/test_default_store.cpp"

    REQUIRES
    "unity"
    "ocs_io"
    "ocs_test"
)
