#!/usr/bin/env bash

set -xe

output=$("$CONTROL_COMPONENTS_PATH"/tools/scripts/verify_license.py --path "$PROJECT_PATH" 2>&1 || exit 1)
if [ -n "$output" ]; then
  echo "Error: Some files are missing the MPL license header:"
  echo "$output"
  exit 1
fi
