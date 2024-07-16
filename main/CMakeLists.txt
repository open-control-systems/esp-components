idf_component_register(
    SRCS
    "main.cpp"

    REQUIRES
    "ocs_core"
    "ocs_status"
    "ocs_net"
    "ocs_storage"
    "ocs_iot"

    INCLUDE_DIRS
    "."
)
