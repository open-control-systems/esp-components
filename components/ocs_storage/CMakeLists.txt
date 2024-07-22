idf_component_register(
    SRCS
    "flash_initializer.cpp"
    "ns_storage.cpp"
    "storage_builder.cpp"

    REQUIRES
    "nvs_flash"

    INCLUDE_DIRS
    ".."
)
