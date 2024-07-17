idf_component_register(
    SRCS
    "cjson_object_formatter.cpp"
    "cjson_array_formatter.cpp"
    "network_json_formatter.cpp"
    "fanout_json_formatter.cpp"
    "system_json_formatter.cpp"
    "string_json_formatter.cpp"
    "version_json_formatter.cpp"

    REQUIRES
    "ocs_net"
    "ocs_core"
    "json"
    "freertos"

    INCLUDE_DIRS
    ".."
)
