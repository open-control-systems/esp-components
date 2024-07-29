/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <cstring>

#include "ocs_diagnostic/basic_counter.h"

namespace ocs {
namespace diagnostic {

BasicCounter::BasicCounter(const char* id) {
    configASSERT(id);
    configASSERT(strlen(id));

    memset(buf_, 0, sizeof(buf_));
    strncpy(buf_, id, std::min(bufsize_, strlen(id)));
}

const char* BasicCounter::id() const {
    return buf_;
}

} // namespace diagnostic
} // namespace ocs
