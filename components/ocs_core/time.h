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
namespace core {

//! Microseconds.
using Time = int64_t;

//! One microsecond represented in microseconds.
const Time Microsecond = 1;

//! One millisecond represented in microseconds.
const Time Millisecond = 1000 * Microsecond;

//! One second represented in microseconds.
const Time Second = 1000 * Millisecond;

//! One minute represented in microseconds.
const Time Minute = 60 * Second;

//! One hour represented in microseconds.
const Time Hour = 60 * Minute;

} // namespace core
} // namespace ocs
