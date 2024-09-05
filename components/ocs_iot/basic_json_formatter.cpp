/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_iot/basic_json_formatter.h"

namespace ocs {
namespace iot {

BasicJsonFormatter::BasicJsonFormatter(bool flat_formatting)
    : flat_formatting_(flat_formatting) {
}

} // namespace iot
} // namespace ocs
