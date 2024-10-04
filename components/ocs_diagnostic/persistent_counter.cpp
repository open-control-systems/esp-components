/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_diagnostic/persistent_counter.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace diagnostic {

namespace {

const char* log_tag = "persistent_counter";

} // namespace

PersistentCounter::PersistentCounter(storage::IStorage& storage, ICounter& counter)
    : storage_(storage)
    , counter_(counter) {
    const auto code = storage_.read(counter_.id(), &value_, sizeof(value_));
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        ocs_loge(log_tag, "failed to read initial counter value: id=%s code=%s",
                 counter_.id(), status::code_to_str(code));
    }
}

const char* PersistentCounter::id() const {
    return counter_.id();
}

ICounter::Value PersistentCounter::get() const {
    return value_ + counter_.get();
}

void PersistentCounter::handle_reboot() {
    const auto code = save();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to persist counter value on reboot: id=%s code=%s",
                 counter_.id(), status::code_to_str(code));
    }
}

status::StatusCode PersistentCounter::run() {
    return save();
}

status::StatusCode PersistentCounter::save() {
    const auto value = get();
    return storage_.write(counter_.id(), &value, sizeof(value));
}

status::StatusCode PersistentCounter::invalidate() {
    value_ = 0;
    return storage_.erase(id());
}

} // namespace diagnostic
} // namespace ocs
