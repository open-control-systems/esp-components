/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_system/ireboot_handler.h"

namespace ocs {
namespace system {

class FanoutRebootHandler : public IRebootHandler, public core::NonCopyable<> {
public:
    //! Deliver reboot event to the underlying handlers.
    void handle_reboot() override;

    //! Add handler to be notified when the reboot event is happened.
    void add(IRebootHandler& handler);

private:
    std::vector<IRebootHandler*> handlers_;
};

} // namespace system
} // namespace ocs
