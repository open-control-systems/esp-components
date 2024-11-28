idf_component_register(
    SRCS
    "server.cpp"
    "client_builder.cpp"
    "client_reader.cpp"
    "chunk_stream_writer.cpp"

    REQUIRES
    "esp_http_server"
    "esp_http_client"
    "ocs_core"
    "ocs_net"

    INCLUDE_DIRS
    ".."
)
