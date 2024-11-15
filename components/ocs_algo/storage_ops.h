/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_storage/istorage.h"

namespace ocs {
namespace algo {

struct StorageOps {
    //! Read string with @p key from @p storage to @p buf which can hold @p size bytes.
    static status::StatusCode
    prob_read(storage::IStorage& storage, const char* key, void* buf, unsigned size);
};

} // namespace algo
} // namespace ocs
