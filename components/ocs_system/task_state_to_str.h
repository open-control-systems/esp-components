/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace ocs {
namespace system {

//! Return human-readable FreeRTOS task state description.
const char* task_state_to_str(eTaskState state);

} // namespace system
} // namespace ocs
