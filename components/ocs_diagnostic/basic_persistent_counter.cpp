/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_diagnostic/basic_persistent_counter.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace diagnostic {

namespace {

const char* log_tag = "basic_persistent_counter";

} // namespace

BasicPersistentCounter::BasicPersistentCounter(storage::IStorage& storage,
                                               ICounter& counter)
    : counter_(counter)
    , storage_(storage) {
    const auto code = storage_.read(counter_.id(), &value_, sizeof(value_));
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        ocs_loge(log_tag, "failed to read initial counter value: id=%s code=%s",
                 counter_.id(), status::code_to_str(code));
    }
}

const char* BasicPersistentCounter::id() const {
    return counter_.id();
}

void BasicPersistentCounter::handle_reboot() {
    const auto code = save_();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to persist counter value on reboot: id=%s code=%s",
                 counter_.id(), status::code_to_str(code));
    }
}

status::StatusCode BasicPersistentCounter::run() {
    return save_();
}

status::StatusCode BasicPersistentCounter::save_() {
    const auto value = get();
    return storage_.write(counter_.id(), &value, sizeof(value));
}

} // namespace diagnostic
} // namespace ocs
