idf_component_register(
    SRCS
    "ds18b20/http_handler.cpp"
    "ds18b20/json_formatter.cpp"

    REQUIRES
    "ocs_io"
    "ocs_net"
    "ocs_iot"
    "ocs_core"
    "ocs_status"
    "ocs_sensor"
    "ocs_onewire"

    INCLUDE_DIRS
    ".."
)
