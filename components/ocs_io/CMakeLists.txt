idf_component_register(
    SRCS
    "oneshot_gpio.cpp"
    "default_gpio.cpp"
    "basic_gpio.cpp"
    "delay_gpio.cpp"

    REQUIRES
    "driver"
    "ocs_scheduler"
    "ocs_core"
    "ocs_status"

    INCLUDE_DIRS
    ".."
)
