/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_storage/flash_initializer.h"
#include "ocs_storage/storage_builder.h"

namespace ocs {
namespace storage {

TEST_CASE("Storage building validation", "[ocs_storage], [storage_builder]") {
    FlashInitializer initializer;

    StorageBuilder builder;

    const char* key = "foo";

    TEST_ASSERT_NOT_NULL(builder.make(key));
    TEST_ASSERT_NULL(builder.make(key));
}

} // namespace storage
} // namespace ocs
