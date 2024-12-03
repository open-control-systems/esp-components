idf_component_register(
    SRCS
    "json/basic_formatter.cpp"
    "json/cjson_array_formatter.cpp"
    "json/cjson_object_formatter.cpp"
    "json/dynamic_formatter.cpp"
    "json/fanout_formatter.cpp"
    "json/field_formatter.cpp"
    "json/string_formatter.cpp"
    "json/func_formatter.cpp"
    "json/time_formatter.cpp"

    REQUIRES
    "json"
    "ocs_core"

    INCLUDE_DIRS
    ".."
)
