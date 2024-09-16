# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

list(APPEND EXTRA_COMPONENT_DIRS "../components")

set(TEST_COMPONENTS
    ocs_net
    ocs_core
    ocs_storage
    ocs_diagnostic
    ocs_scheduler
    ocs_io
    ocs_system
    ocs_sensor
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(unittests)
