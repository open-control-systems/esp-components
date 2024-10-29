/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_system/reboot_task.h"

namespace ocs {
namespace system {

RebootTask::RebootTask(IRebooter& rebooter)
    : rebooter_(rebooter) {
}

status::StatusCode RebootTask::run() {
    rebooter_.reboot();
    return status::StatusCode::OK;
}

} // namespace system
} // namespace ocs
