idf_component_register(
    SRCS
    "test_basic_counter.cpp"
    "test_time_counter.cpp"
    "test_mem_persistent_counter.cpp"
    "test_acc_persistent_counter.cpp"

    REQUIRES
    "unity"
    "ocs_test"
    "ocs_diagnostic"
)
