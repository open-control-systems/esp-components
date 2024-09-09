/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_status/code_to_str.h"

namespace ocs {
namespace status {

const char* code_to_str(StatusCode status) {
    switch (status) {
    case StatusCode::OK:
        return "OK";
    case StatusCode::Error:
        return "Error";
    case StatusCode::NoData:
        return "NoData";
    case StatusCode::NoMem:
        return "NoMem";
    case StatusCode::IsrYield:
        return "IsrYield";
    case StatusCode::InvalidArg:
        return "InvalidArg";
    case StatusCode::InvalidState:
        return "InvalidState";
    case StatusCode::Timeout:
        return "Timeout";

    default:
        break;
    }

    return "<none>";
}

} // namespace status
} // namespace ocs
