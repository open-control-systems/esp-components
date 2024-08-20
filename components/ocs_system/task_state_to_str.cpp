/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_system/task_state_to_str.h"

namespace ocs {
namespace system {

const char* task_state_to_str(eTaskState state) {
    switch (state) {
    case eRunning:
        return "Running";

    case eReady:
        return "Ready";

    case eBlocked:
        return "Blocked";

    case eSuspended:
        return "Suspended";

    case eDeleted:
        return "Deleted";

    case eInvalid:
        break;

    default:
        break;
    }

    return "<none>";
}

} // namespace system
} // namespace ocs
