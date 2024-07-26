/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/basic_counter.h"

namespace ocs {
namespace test {

struct TestCounter : public diagnostic::BasicCounter, public core::NonCopyable<> {
    explicit TestCounter(const char* id);

    diagnostic::ICounter::Value get() const override;

    diagnostic::ICounter::Value value { 0 };
};

} // namespace test
} // namespace ocs
