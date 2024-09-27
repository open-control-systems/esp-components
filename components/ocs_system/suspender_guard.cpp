/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_system/suspender_guard.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "suspender-guard";

} // namespace

SuspenderGuard::SuspenderGuard(ISuspender& suspender)
    : suspender_(suspender) {
    const auto code = suspender_.suspend();
    if (code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to suspend: %s", status::code_to_str(code));
    }
}

SuspenderGuard::~SuspenderGuard() {
    const auto code = suspender_.resume();
    if (code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to resume: %s", status::code_to_str(code));
    }
}

} // namespace system
} // namespace ocs
