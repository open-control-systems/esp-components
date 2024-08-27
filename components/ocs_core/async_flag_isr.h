/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/basic_async_flag.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

//! Implements a sort of condition variable, a rendezvous point for the
//! FreeRTOS task, waiting for the asynchronous event to be occurred, and the ISR,
//! signaling the FreeRTOS task about the asynchronous event.
class AsyncFlagIsr : public BasicAsyncFlag, public NonCopyable<> {
public:
    //! Signal the asynchronous event from ISR.
    status::StatusCode signal() override;
};

} // namespace core
} // namespace ocs
