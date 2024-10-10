idf_component_register(
    SRCS
    "master_transmitter.cpp"

    REQUIRES
    "freertos"
    "driver"
    "ocs_core"
    "ocs_status"

    INCLUDE_DIRS
    ".."
)
