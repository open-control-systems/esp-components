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
using microseconds_t = int64_t;

//! One microsecond represented in microseconds.
const microseconds_t Microsecond = 1;

//! One millisecond represented in microseconds.
const microseconds_t Millisecond = 1000 * Microsecond;

//! One second represented in microseconds.
const microseconds_t Second = 1000 * Millisecond;

//! One minute represented in microseconds.
const microseconds_t Minute = 60 * Second;

//! One hour represented in microseconds.
const microseconds_t Hour = 60 * Minute;

} // namespace core
} // namespace ocs
