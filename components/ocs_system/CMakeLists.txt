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
    "delayer_configuration.cpp"
    "basic_delayer.cpp"
    "default_delayer.cpp"
    "busy_loop_delayer.cpp"
    "low_power_delayer.cpp"
    "default_randomizer.cpp"
    "fanout_suspender.cpp"
    "suspender_guard.cpp"

    REQUIRES
    "esp_timer"
    "driver"

    INCLUDE_DIRS
    ".."
)
