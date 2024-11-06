#!/usr/bin/env bash

set -xe

find $PROJECT_PATH \
    -type f -name "*.cpp" -o -name "*.h" \
    | xargs sha256sum \
    | sha256sum \
    | awk '{print $1}'
