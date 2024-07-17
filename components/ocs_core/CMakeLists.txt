idf_component_register(
    SRCS
    "static_mutex.cpp"
    "static_event_group.cpp"
    "cond.cpp"
    "version.cpp"
    "version_to_str.cpp"

    REQUIRES
    "json"
    "freertos"

    INCLUDE_DIRS
    ".."
)
