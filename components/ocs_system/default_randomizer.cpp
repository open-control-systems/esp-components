/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "esp_random.h"

#include "ocs_system/default_randomizer.h"

namespace ocs {
namespace system {

uint32_t DefaultRandomizer::random(uint32_t from, uint32_t to) {
    configASSERT(from <= to);

    const uint64_t range = static_cast<uint64_t>(to) - from + 1;

    uint64_t x = 0;
    uint64_t r = 0;

    do {
        uint32_t val = esp_random();
        x = static_cast<uint64_t>(val);
        r = x % range;
    } while (x - r > (-range));

    const uint32_t ret = from + static_cast<uint32_t>(r);

    configASSERT(ret >= from);
    configASSERT(ret <= to);

    return ret;
}

} // namespace system
} // namespace ocs
