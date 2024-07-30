/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itimer.h"

namespace ocs {
namespace test {

class TestTimer : public scheduler::ITimer, public core::NonCopyable<> {
public:
    explicit TestTimer(status::StatusCode code);

    status::StatusCode start() override;
    status::StatusCode stop() override;

    unsigned start_call_count { 0 };
    unsigned stop_call_count { 0 };

private:
    status::StatusCode code_ { status::StatusCode::OK };
};

} // namespace test
} // namespace ocs
