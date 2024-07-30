/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace system {

class RebootTask : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Initialize.
    explicit RebootTask(IRebooter& rebooter);

    //! Initiate a reboot process.
    status::StatusCode run() override;

private:
    IRebooter& rebooter_;
};

} // namespace system
} // namespace ocs
