idf_component_register(
    SRCS
    "test_basic_counter.cpp"
    "test_persistent_counter.cpp"
    "test_time_counter.cpp"
    "test_live_counter.cpp"

    REQUIRES
    "unity"
    "ocs_test"
    "ocs_diagnostic"
)
