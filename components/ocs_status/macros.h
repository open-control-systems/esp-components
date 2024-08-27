/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "esp_compiler.h"

#include "ocs_status/code.h"

//! Macro to check the status code. If the status code is not OK, it returns.
#define OCS_STATUS_RETURN_ON_ERROR(x)                                                    \
    do {                                                                                 \
        const ocs::status::StatusCode _code = (x);                                       \
        if (unlikely(_code != ocs::status::StatusCode::OK)) {                            \
            return _code;                                                                \
        }                                                                                \
    } while (0)

#define OCS_STATUS_RETURN_ON_FALSE(x, code)                                              \
    do {                                                                                 \
        if (unlikely(!(x))) {                                                            \
            return code;                                                                 \
        }                                                                                \
    } while (0)
