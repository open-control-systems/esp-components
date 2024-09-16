/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>

namespace ocs {
namespace onewire {

//! Calculate CRC-8 for Dallas Semicondactor devices.
//!
//! @remarks
//!  @p buf should be at least @p size bytes long.
//!
//! @references
//!  https://stackoverflow.com/questions/29214301/ios-how-to-calculate-crc-8-dallas-maxim-of-nsdata
//!  https://stackoverflow.com/questions/74775595/maxim-dow-crc-algorithm-cannot-re-create-example-in-application-note
//!  https://www.boost.org/doc/libs/1_84_0/doc/html/crc/crc_optimal.html
uint8_t calculate_crc(const uint8_t* buf, unsigned size);

} // namespace onewire
} // namespace ocs
