idf_component_register(
    SRCS
    "fanout_reboot_handler.cpp"
    "default_clock.cpp"
    "default_rebooter.cpp"
    "delay_rebooter.cpp"

    REQUIRES
    "esp_timer"

    INCLUDE_DIRS
    ".."
)
