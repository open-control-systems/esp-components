idf_component_register(
    SRCS
    "test_static_mutex.cpp"
    "test_version.cpp"
    "test_version_to_str.cpp"
    "test_cond.cpp"
    "test_rate_limiter.cpp"

    REQUIRES
    "unity"
    "ocs_core"
)
