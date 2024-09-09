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

    //! A context switch should be requested before the interrupt is exited.
    IsrYield,

    //! An operation can't be performed due to invalid arguments.
    InvalidArg,

    //! An operation can't be performed due to invalid state.
    InvalidState,

    //! An operation was not performed within the timeout.
    Timeout,

    //! Invalid status code.
    Last,
};

} // namespace status
} // namespace ocs
