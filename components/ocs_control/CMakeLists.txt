idf_component_register(
    SRCS
    "fsm_store.cpp"
    "fsm_block.cpp"
    "fsm_block_pipeline.cpp"

    REQUIRES
    "freertos"
    "ocs_core"
    "ocs_scheduler"
    "ocs_storage"

    INCLUDE_DIRS
    ".."
)
