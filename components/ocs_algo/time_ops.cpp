/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include "ocs_algo/time_ops.h"
#include "ocs_core/log.h"

namespace ocs {
namespace algo {

namespace {

const char* log_tag = "time_ops";

} // namespace

std::optional<time_t> TimeOps::get_time() {
    const time_t timestamp = time(nullptr);
    if (timestamp == -1) {
        ocs_loge(log_tag, "time(): %s", std::strerror(errno));

        return std::nullopt;
    }

    return timestamp;
}

} // namespace algo
} // namespace ocs
