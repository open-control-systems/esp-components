#!/usr/bin/env bash

set -xe

files=$(find "$PROJECT_PATH" \
    -type f \( -name '*.cpp' -o -name '*.h' \) \
    ! -path '*/build/*' \
    ! -path '*/managed_components/*')

clang-format --dry-run --Werror -style=file $files
