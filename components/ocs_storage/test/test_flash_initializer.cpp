/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_storage/flash_initializer.h"

namespace ocs {
namespace storage {

TEST_CASE("Flash initializer", "[ocs_storage], [flash_initializer]") {
    FlashInitializer initializer;
}

} // namespace storage
} // namespace ocs
