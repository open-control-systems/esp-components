idf_component_register(
    SRCS
    "httpserver/http_pipeline.cpp"
    "httpserver/ds18b20_handler.cpp"
    "httpserver/sht41_handler.cpp"
    "httpserver/data_handler.cpp"
    "httpserver/system_handler.cpp"
    "httpserver/system_state_handler.cpp"

    "jsonfmt/data_pipeline.cpp"
    "jsonfmt/network_formatter.cpp"
    "jsonfmt/system_formatter.cpp"
    "jsonfmt/counter_formatter.cpp"
    "jsonfmt/telemetry_formatter.cpp"
    "jsonfmt/registration_formatter.cpp"
    "jsonfmt/version_formatter.cpp"
    "jsonfmt/system_state_formatter.cpp"
    "jsonfmt/console_task.cpp"
    "jsonfmt/console_pipeline.cpp"
    "jsonfmt/sht41_sensor_formatter.cpp"
    "jsonfmt/bme280_sensor_formatter.cpp"
    "jsonfmt/ldr_sensor_formatter.cpp"
    "jsonfmt/ds18b20_sensor_formatter.cpp"
    "jsonfmt/soil_analog_sensor_formatter.cpp"

    "basic/system_pipeline.cpp"
    "basic/network_pipeline.cpp"
    "basic/mdns_pipeline.cpp"
    "basic/system_counter_pipeline.cpp"

    REQUIRES
    "ocs_io"
    "ocs_net"
    "ocs_http"
    "ocs_core"
    "ocs_status"
    "ocs_sensor"
    "ocs_onewire"
    "ocs_diagnostic"
    "ocs_storage"
    "ocs_fmt"
    "ocs_control"
    "ocs_algo"

    INCLUDE_DIRS
    ".."
)
