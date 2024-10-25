/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/idelayer.h"

namespace ocs {
namespace system {

class DefaultDelayer : public IDelayer, public core::NonCopyable<> {
public:
    //! Highly-accurate delay based on the builtin ets_delay_us().
    status::StatusCode delay(core::Time delay) override;
};

} // namespace system
} // namespace ocs
