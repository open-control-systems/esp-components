/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "driver/gptimer.h"

#include "ocs_core/noncopyable.h"
#include "ocs_system/idelayer.h"

namespace ocs {
namespace system {

class BasicDelayer : public IDelayer, public core::NonCopyable<BasicDelayer> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p resolution - single tick duration equals to 1/resolution.
    explicit BasicDelayer(core::Time resolution);

    //! Destroy.
    virtual ~BasicDelayer();

protected:
    gptimer_handle_t handle_ { nullptr };
};

} // namespace system
} // namespace ocs
