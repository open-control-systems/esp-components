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
#include "ocs_diagnostic/icounter.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace diagnostic {

struct TestCounterStorage : public storage::IStorage, public core::NonCopyable<> {
    status::StatusCode read(const char* id, void* value, size_t size) override;
    status::StatusCode write(const char* id, const void* value, size_t size) override;
    status::StatusCode erase(const char* id) override;

    void set(const char* key, ICounter::Value value);
    std::optional<ICounter::Value> get(const char* key);

private:
    std::unordered_map<std::string, ICounter::Value> values_;
};

} // namespace diagnostic
} // namespace ocs
