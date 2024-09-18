idf_component_register(
    SRCS
    "main.cpp"

    REQUIRES
    "ocs_core"
    "ocs_status"
    "ocs_net"
    "ocs_storage"
    "ocs_system"
    "ocs_diagnostic"
    "ocs_test"
    "ocs_scheduler"
    "ocs_io"
    "ocs_sensor"
    "ocs_pipeline"
    "ocs_onewire"
    "ocs_fmt"

    INCLUDE_DIRS
    "."
)
