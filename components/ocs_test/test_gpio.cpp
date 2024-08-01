/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_test/test_gpio.h"

namespace ocs {
namespace test {

TestGpio::TestGpio(status::StatusCode flip_code,
                   status::StatusCode turn_on_code,
                   status::StatusCode turn_off_code)
    : flip_code_(flip_code)
    , turn_on_code_(turn_on_code)
    , turn_off_code_(turn_off_code) {
}

status::StatusCode TestGpio::flip() {
    ++flip_call_count;

    return flip_code_;
}

status::StatusCode TestGpio::turn_on() {
    ++turn_on_call_count;

    return turn_on_code_;
}

status::StatusCode TestGpio::turn_off() {
    ++turn_off_call_count;

    return turn_off_code_;
}

} // namespace test
} // namespace ocs
