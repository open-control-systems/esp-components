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
//! FreeRTOS tasks, waiting and signaling the asynchronous events.
class AsyncFlag : public BasicAsyncFlag, public NonCopyable<> {
public:
    //! Signal the asynchronous event from FreeRTOS task.
    status::StatusCode signal() override;
};

} // namespace core
} // namespace ocs
