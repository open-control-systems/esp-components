idf_component_register(
    SRCS
    "test_clock.cpp"
    "test_counter.cpp"
    "test_counter_storage.cpp"
    "test_task.cpp"
    "test_timer.cpp"
    "test_gpio.cpp"

    REQUIRES
    "unity"
    "ocs_core"
    "ocs_status"
    "ocs_storage"
    "ocs_diagnostic"
    "ocs_scheduler"
    "ocs_io"

    INCLUDE_DIRS
    ".."
)
