idf_component_register(
    SRCS
    "test_default_delayer.cpp"
    "test_busy_loop_delayer.cpp"
    "test_low_power_delayer.cpp"
    "test_fanout_suspender.cpp"

    REQUIRES
    "unity"
    "ocs_system"
)
