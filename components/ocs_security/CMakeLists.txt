idf_component_register(
    SRCS
    "sha_generator.cpp"
    "sha_to_hex_str.cpp"
    "basic_sha_engine.cpp"
    "mbedtls_sha_engine.cpp"

    REQUIRES
    "mbedtls"
    "ocs_status"
    "ocs_algo"

    INCLUDE_DIRS
    ".."
)
