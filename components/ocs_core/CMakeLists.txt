idf_component_register(
    SRCS
    "static_mutex.cpp"
    "static_event_group.cpp"
    "cond.cpp"

    REQUIRES
    "json"
    "freertos"

    INCLUDE_DIRS
    ".."
)
