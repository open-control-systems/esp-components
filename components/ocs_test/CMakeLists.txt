idf_component_register(
    SRCS
    "test_clock.cpp"
    "test_counter.cpp"
    "test_counter_storage.cpp"

    REQUIRES
    "unity"
    "ocs_core"
    "ocs_status"
    "ocs_storage"
    "ocs_diagnostic"

    INCLUDE_DIRS
    ".."
)
