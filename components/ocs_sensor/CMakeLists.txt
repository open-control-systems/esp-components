idf_component_register(
    SRCS
    "yl69/sensor.cpp"
    "yl69/relay_sensor.cpp"
    "yl69/default_sensor_task.cpp"
    "yl69/safe_sensor_task.cpp"
    "ldr/sensor.cpp"
    "ldr/sensor_task.cpp"
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
