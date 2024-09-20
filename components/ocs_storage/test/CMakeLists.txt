idf_component_register(
    SRCS
    "test_flash_initializer.cpp"
    "test_nvs_storage.cpp"
    "test_storage_builder.cpp"

    REQUIRES
    "unity"
    "ocs_storage"
)
