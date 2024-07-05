idf_component_register(
    SRCS
    "cjson_object_formatter.cpp"
    "cjson_array_formatter.cpp"
    "static_mutex.cpp"
    "static_event_group.cpp"

    REQUIRES
    "json"
    "freertos"

    INCLUDE_DIRS
    ".."
)
