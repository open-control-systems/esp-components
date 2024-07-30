/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_test/test_timer.h"

namespace ocs {
namespace test {

TestTimer::TestTimer(status::StatusCode code)
    : code_(code) {
}

status::StatusCode TestTimer::start() {
    ++start_call_count;

    return code_;
}

status::StatusCode TestTimer::stop() {
    ++stop_call_count;

    return code_;
}

} // namespace test
} // namespace ocs
