## Introduction

`esp-components` is a C++ library dedicated to be used in various ESP-IDF based projects.

## Installation

All components are located in the `components` directory. It's a default directory name where the ESP-IDF build system looks up for the project libraries, or components in terms of the ESP-IDF SDK. See the build system [documentation](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/build-system.html#component-cmakelists-files) for more details.

These components can be manually copied into the project `components` directory, or added as a git submodule:

```bash
git submodule add git@github.com:open-control-systems/esp-components.git ocs-esp-components
git submodule update --init --recursive
```

and then the project's root `CMakeLists.txt` should be updated file as follows:

```CMake
# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS
    "ocs-esp-components/components"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(main)
```

## License

This project is licensed under the MPL 2.0 License - see the LICENSE file for details.
