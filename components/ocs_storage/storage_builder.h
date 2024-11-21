/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <set>
#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace storage {

class StorageBuilder : public core::NonCopyable<> {
public:
    using IStoragePtr = std::unique_ptr<IStorage>;

    //! Create a storage with a unique @ id.
    //!
    //! @return
    //!  nullptr if storage with @p id already exists.
    IStoragePtr make(const char* id);

private:
    std::set<std::string> ids_;
};

} // namespace storage
} // namespace ocs
