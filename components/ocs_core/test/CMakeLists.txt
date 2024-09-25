idf_component_register(
    SRCS
    "test_static_mutex.cpp"
    "test_version.cpp"
    "test_version_to_str.cpp"
    "test_cond.cpp"
    "test_string_ops.cpp"
    "test_time_ops.cpp"

    REQUIRES
    "unity"
    "ocs_core"
)
