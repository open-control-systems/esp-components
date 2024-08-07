idf_component_register(
    SRCS
    "main.cpp"

    REQUIRES
    "ocs_core"
    "ocs_status"
    "ocs_net"
    "ocs_storage"
    "ocs_iot"
    "ocs_system"
    "ocs_diagnostic"
    "ocs_test"
    "ocs_scheduler"
    "ocs_io"
    "ocs_sensor"

    INCLUDE_DIRS
    "."
)
