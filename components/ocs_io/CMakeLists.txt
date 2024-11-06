idf_component_register(
    SRCS
    "gpio/default_gpio.cpp"
    "gpio/basic_gpio.cpp"
    "gpio/delay_gpio.cpp"
    "gpio/gpio_guard.cpp"

    "adc/oneshot_adc.cpp"
    "adc/default_store.cpp"

    REQUIRES
    "driver"
    "esp_adc"
    "ocs_scheduler"
    "ocs_core"
    "ocs_status"

    INCLUDE_DIRS
    ".."
)
