idf_component_register(
    SRCS
    "test_basic_counter.cpp"
    "test_persistent_counter.cpp"
    "test_time_counter.cpp"
    "test_live_counter.cpp"
    "test_counter.cpp"
    "test_counter_storage.cpp"
    "test_clock.cpp"

    REQUIRES
    "unity"
    "ocs_iot"
)
