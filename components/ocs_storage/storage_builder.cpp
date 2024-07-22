/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_storage/storage_builder.h"
#include "ocs_storage/ns_storage.h"

namespace ocs {
namespace storage {

StoragePtr StorageBuilder::make(const char* id) {
    auto [it, ok] = ids_.insert(id);
    if (!ok) {
        return nullptr;
    }

    return StoragePtr(new (std::nothrow) NsStorage(id));
}

} // namespace storage
} // namespace ocs
