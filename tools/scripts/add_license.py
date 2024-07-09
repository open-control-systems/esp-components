#!/usr/bin/env python

import sys

if sys.version_info[0] < 3:
    raise Exception("Python 3 or a more recent version is required.")

import os
import argparse
from datetime import date

license_text = f"""/*
 * Copyright (c) {date.today().year}, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */"""


def add_license_header(args: argparse.Namespace):
    for root, dirs, files in os.walk(args.path):
        for file_name in files:
            if file_name.endswith(".cpp") or file_name.endswith(".h"):
                file_path = os.path.join(root, file_name)

                with open(file_path, 'r+') as file:
                    content = file.read()
                    if not content.startswith("/*"):
                        file.seek(0, 0)
                        file.write(license_text.rstrip('\r\n') + '\n\n' + content)

                        print(f'LICENSED MPL 2.0: {file_name}', file=sys.stderr)


def parse_args():
    parser = argparse.ArgumentParser(description='Add MPL 2.0 License Header')

    parser.add_argument('--path', help='Project path', required=True)

    return parser.parse_args()


def main() -> None:
    args: argparse.Namespace = parse_args()
    add_license_header(args)


if __name__ == '__main__':
    main()
