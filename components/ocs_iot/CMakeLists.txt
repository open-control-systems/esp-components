idf_component_register(
    SRCS
    "cjson_object_formatter.cpp"
    "cjson_array_formatter.cpp"
    "network_json_formatter.cpp"
    "fanout_json_formatter.cpp"
    "system_json_formatter.cpp"
    "string_json_formatter.cpp"
    "version_json_formatter.cpp"
    "system_counter_pipeline.cpp"
    "counter_json_formatter.cpp"
    "http_server_pipeline.cpp"
    "system_pipeline.cpp"
    "telemetry_json_formatter.cpp"
    "registration_json_formatter.cpp"
    "json_data_pipeline.cpp"

    REQUIRES
    "ocs_net"
    "ocs_core"
    "ocs_diagnostic"
    "ocs_storage"
    "json"
    "freertos"

    INCLUDE_DIRS
    ".."
)
