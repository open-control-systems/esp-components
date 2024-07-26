/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_test/test_counter.h"

namespace ocs {
namespace test {

TestCounter::TestCounter(const char* id)
    : diagnostic::BasicCounter(id) {
}

diagnostic::ICounter::Value TestCounter::get() const {
    return value;
}

} // namespace test
} // namespace ocs
