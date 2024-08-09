/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_test/test_counter_storage.h"

namespace ocs {
namespace test {

status::StatusCode TestCounterStorage::read(const char* id, void* value, size_t size) {
    TEST_ASSERT_EQUAL(sizeof(diagnostic::ICounter::Value), size);

    const auto read_value = get(id);
    if (!read_value) {
        return status::StatusCode::NoData;
    }

    *static_cast<diagnostic::ICounter::Value*>(value) = *read_value;

    return status::StatusCode::OK;
}

status::StatusCode
TestCounterStorage::write(const char* id, const void* value, size_t size) {
    TEST_ASSERT_EQUAL(sizeof(diagnostic::ICounter::Value), size);

    set(id, *static_cast<const diagnostic::ICounter::Value*>(value));

    return status::StatusCode::OK;
}

status::StatusCode TestCounterStorage::erase(const char* id) {
    const auto num = values_.erase(id);
    TEST_ASSERT_TRUE(num <= 1);

    return num == 1 ? status::StatusCode::OK : status::StatusCode::NoData;
}

void TestCounterStorage::set(const char* key, diagnostic::ICounter::Value value) {
    values_[key] = value;
}

std::optional<diagnostic::ICounter::Value> TestCounterStorage::get(const char* key) {
    const auto it = values_.find(key);
    if (it == values_.end()) {
        return std::nullopt;
    }

    return it->second;
}

} // namespace test
} // namespace ocs