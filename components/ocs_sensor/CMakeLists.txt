idf_component_register(
    SRCS
    "yl69_sensor.cpp"
    "relay_sensor.cpp"
    "default_yl69_sensor_task.cpp"
    "safe_yl69_sensor_task.cpp"
    "ldr_sensor.cpp"
    "ldr_sensor_task.cpp"
    "ds18b20/store.cpp"
    "ds18b20/sensor.cpp"
    "ds18b20/scratchpad.cpp"
    "ds18b20/sensor_task.cpp"
    "ds18b20/resolution_to_str.cpp"
    "ds18b20/parse_configuration.cpp"

    REQUIRES
    "ocs_io"
    "ocs_core"
    "ocs_status"
    "ocs_system"
    "ocs_storage"
    "ocs_scheduler"
    "ocs_diagnostic"
    "ocs_onewire"

    INCLUDE_DIRS
    ".."
)
