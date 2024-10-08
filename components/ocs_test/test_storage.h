/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "unity.h"

#include "ocs_core/noncopyable.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace test {

template <typename T>
struct TestStorage : public storage::IStorage, public core::NonCopyable<> {
    status::StatusCode read(const char* id, void* value, size_t size) {
        TEST_ASSERT_EQUAL(sizeof(T), size);

        const auto read_value = get(id);
        if (!read_value) {
            return status::StatusCode::NoData;
        }

        *static_cast<T*>(value) = *read_value;

        return status::StatusCode::OK;
    }

    status::StatusCode write(const char* id, const void* value, size_t size) {
        TEST_ASSERT_EQUAL(sizeof(T), size);

        set(id, *static_cast<const T*>(value));

        return status::StatusCode::OK;
    }

    status::StatusCode erase(const char* id) {
        const auto num = values_.erase(id);
        TEST_ASSERT_TRUE(num <= 1);

        return num == 1 ? status::StatusCode::OK : status::StatusCode::NoData;
    }

    void set(const char* key, T value) {
        values_[key] = value;
    }

    std::optional<T> get(const char* key) {
        const auto it = values_.find(key);
        if (it == values_.end()) {
            return std::nullopt;
        }

        return it->second;
    }

private:
    std::unordered_map<std::string, T> values_;
};

} // namespace test
} // namespace ocs
