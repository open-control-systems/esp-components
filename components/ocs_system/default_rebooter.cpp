/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "esp_system.h"

#include "ocs_core/log.h"
#include "ocs_system/default_rebooter.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "default_rebooter";

} // namespace

DefaultRebooter::DefaultRebooter(IRebootHandler& handler)
    : handler_(handler) {
}

void DefaultRebooter::reboot() {
    ocs_logi(log_tag, "Prepare for rebooting...");
    handler_.handle_reboot();
    ocs_logi(log_tag, "Ready for rebooting...");

    return esp_restart();
}

} // namespace system
} // namespace ocs
