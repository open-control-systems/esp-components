idf_component_register(
    SRCS
    "basic_static_mutex.cpp"
    "static_mutex.cpp"
    "static_mutex_isr.cpp"
    "basic_static_recursive_mutex.cpp"
    "static_recursive_mutex.cpp"
    "static_event_group.cpp"
    "cond.cpp"
    "lock_guard.cpp"
    "basic_async_flag.cpp"
    "async_flag.cpp"
    "async_flag_isr.cpp"
    "version.cpp"
    "version_to_str.cpp"
    "bit_ops.cpp"
    "future.cpp"
    "string_ops.cpp"
    "rate_limiter.cpp"
    "operation_guard.cpp"

    REQUIRES
    "json"
    "freertos"

    INCLUDE_DIRS
    ".."
)
