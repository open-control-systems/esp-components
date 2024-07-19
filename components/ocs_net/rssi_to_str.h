/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstdint>

#pragma once

namespace ocs {
namespace net {

//! Conver RSSI value into human-readable signal strength.
const char* rssi_to_str(int rssi);

} // namespace net
} // namespace ocs
