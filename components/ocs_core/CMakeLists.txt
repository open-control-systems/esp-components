idf_component_register(
    SRCS
    "basic_static_mutex.cpp"
    "static_mutex.cpp"
    "static_mutex_isr.cpp"
    "static_event_group.cpp"
    "cond.cpp"
    "lock_guard.cpp"
    "basic_async_flag.cpp"
    "async_flag.cpp"
    "async_flag_isr.cpp"
    "version.cpp"
    "version_to_str.cpp"

    REQUIRES
    "json"
    "freertos"

    INCLUDE_DIRS
    ".."
)
