/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/ireboot_handler.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace system {

class DefaultRebooter : public IRebooter, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handler to be notified before the actual reboot is happened.
    explicit DefaultRebooter(IRebootHandler& handler);

    //! Restart the board.
    void reboot() override;

private:
    IRebootHandler& handler_;
};

} // namespace system
} // namespace ocs
