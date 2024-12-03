idf_component_register(
    SRCS
    "crc_ops.cpp"
    "math_ops.cpp"
    "bit_ops.cpp"
    "string_ops.cpp"
    "uri_ops.cpp"
    "storage_ops.cpp"
    "time_ops.cpp"

    REQUIRES
    "ocs_storage"
    "ocs_status"
    "ocs_security"

    INCLUDE_DIRS
    ".."
)
