/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/igpio.h"

namespace ocs {
namespace test {

class TestGpio : public io::IGpio, public core::NonCopyable<> {
public:
    TestGpio(status::StatusCode flip_code,
             status::StatusCode turn_on_code,
             status::StatusCode turn_off_code);

    int get() override;
    status::StatusCode flip() override;
    status::StatusCode turn_on() override;
    status::StatusCode turn_off() override;

    unsigned value { 0 };
    unsigned flip_call_count { 0 };
    unsigned turn_on_call_count { 0 };
    unsigned turn_off_call_count { 0 };

private:
    status::StatusCode flip_code_ { status::StatusCode::OK };
    status::StatusCode turn_on_code_ { status::StatusCode::OK };
    status::StatusCode turn_off_code_ { status::StatusCode::OK };
};

} // namespace test
} // namespace ocs
