idf_component_register(
    SRCS
    "fanout_reboot_handler.cpp"
    "default_clock.cpp"
    "default_rebooter.cpp"
    "delay_rebooter.cpp"
    "reboot_task.cpp"
    "reset_reason_to_str.cpp"
    "task_state_to_str.cpp"
    "system_state_builder.cpp"

    REQUIRES
    "esp_timer"

    INCLUDE_DIRS
    ".."
)
