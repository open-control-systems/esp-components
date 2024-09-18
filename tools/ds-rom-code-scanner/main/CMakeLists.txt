idf_component_register(
    SRCS
    "main.cpp"

    REQUIRES
    "freertos"
    "driver"
    "ocs_onewire"
    "ocs_status"
    "ocs_system"
    "ocs_core"
    "ocs_io"
    "ocs_fmt"

    INCLUDE_DIRS
    "."
)
