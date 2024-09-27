/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/isuspender.h"

namespace ocs {
namespace system {

class SuspenderGuard : public core::NonCopyable<> {
public:
    //! Suspend on initialization.
    explicit SuspenderGuard(ISuspender& suspender);

    //! Resume on de-initialization.
    ~SuspenderGuard();

private:
    ISuspender& suspender_;
};

} // namespace system
} // namespace ocs
