/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "ocs_system/idelayer.h"

namespace ocs {
namespace system {

enum class DelayerStrategy : uint8_t {
    //! Highly-accurate delay strategy based on builtin functions.
    Default,

    //! Highly-accurate delay strategy based on hardware timers and busy loop.
    BusyLoop,

    //! Low power delay strategy based on hardware timers and interrupts.
    LowPower,

    //! Invalid value.
    Last,
};

//! Parse the delayer strategy from the raw value.
std::optional<DelayerStrategy> parse_delayer_strategy(int value);

using IDelayerPtr = std::unique_ptr<IDelayer>;

//! Build delayer based on the @p strategy.
IDelayerPtr make_delayer(DelayerStrategy strategy);

} // namespace system
} // namespace ocs
