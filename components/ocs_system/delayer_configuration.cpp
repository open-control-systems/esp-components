/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_system/delayer_configuration.h"
#include "ocs_system/busy_loop_delayer.h"
#include "ocs_system/default_delayer.h"
#include "ocs_system/low_power_delayer.h"

namespace ocs {
namespace system {

std::optional<DelayerStrategy> parse_delayer_strategy(int value) {
    if (value < 0 || value >= static_cast<int>(DelayerStrategy::Last)) {
        return std::nullopt;
    }

    return static_cast<DelayerStrategy>(value);
}

IDelayerPtr make_delayer(DelayerStrategy strategy) {
    switch (strategy) {
    case system::DelayerStrategy::Default:
        return IDelayerPtr(new (std::nothrow) system::DefaultDelayer());

    case system::DelayerStrategy::BusyLoop:
        return IDelayerPtr(new (std::nothrow)
                               system::BusyLoopDelayer(core::Duration::second));

    case system::DelayerStrategy::LowPower:
        return IDelayerPtr(new (std::nothrow)
                               system::LowPowerDelayer(core::Duration::second));

    default:
        break;
    }

    return nullptr;
}

} // namespace system
} // namespace ocs
