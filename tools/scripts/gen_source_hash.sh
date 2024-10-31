#!/usr/bin/env bash

set -xe

find $PROJECT_PATH \
    -type f -name "*.cpp" -o -name "*.h" \
    | xargs sha512sum \
    | sha512sum \
    | awk '{print $1}'
