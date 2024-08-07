/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace ocs {
namespace status {

//! Status code.
enum class StatusCode {
    //! Status indicating a success of an operation.
    OK,

    //! Status indicating a failure of an operation.
    Error,

    //! There is no enough data to perform an operation.
    NoData,

    //! There is no enough memory to perform an operation.
    NoMem,
};

} // namespace status
} // namespace ocs
