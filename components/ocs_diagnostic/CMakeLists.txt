idf_component_register(
    SRCS
    "basic_counter.cpp"
    "time_counter.cpp"
    "persistent_counter.cpp"
    "live_counter.cpp"
    "basic_counter_holder.cpp"

    REQUIRES
    "ocs_core"
    "ocs_diagnostic"

    INCLUDE_DIRS
    ".."
)
