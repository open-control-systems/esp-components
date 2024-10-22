idf_component_register(
    SRCS
    "master_store.cpp"
    "master_transceiver.cpp"

    REQUIRES
    "freertos"
    "driver"
    "ocs_core"
    "ocs_status"

    INCLUDE_DIRS
    ".."
)
