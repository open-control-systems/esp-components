idf_component_register(
    SRCS
    "crc.cpp"
    "bus.cpp"
    "rom_code.cpp"
    "rom_code_scanner.cpp"
    "serial_number_to_str.cpp"

    REQUIRES
    "ocs_io"
    "ocs_core"
    "ocs_status"
    "ocs_system"

    INCLUDE_DIRS
    ".."
)
