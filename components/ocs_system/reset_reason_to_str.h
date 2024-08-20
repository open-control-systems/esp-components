/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "esp_system.h"

namespace ocs {
namespace system {

//! Return human-readable reset reason description.
const char* reset_reason_to_str(esp_reset_reason_t reason);

} // namespace system
} // namespace ocs
