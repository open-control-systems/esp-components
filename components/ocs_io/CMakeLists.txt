idf_component_register(
    SRCS
    "default_gpio.cpp"
    "basic_gpio.cpp"
    "delay_gpio.cpp"
    "gpio_guard.cpp"
    "oneshot_adc.cpp"
    "adc_store.cpp"

    REQUIRES
    "soc"
    "driver"
    "esp_adc"
    "ocs_scheduler"
    "ocs_core"
    "ocs_status"

    INCLUDE_DIRS
    ".."
)
