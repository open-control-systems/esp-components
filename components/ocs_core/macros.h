/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

//! Structure's fields are packed.
#define OCS_ATTR_PACKED __attribute__((packed))

//! Get number of elements in array.
#define OCS_ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
