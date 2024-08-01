/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/igpio.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace io {

class OneshotGpio : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Initialize.
    OneshotGpio(scheduler::ITask& task, IGpio& gpio);

    //! Switch the GPIO once.
    status::StatusCode run() override;

private:
    scheduler::ITask& task_;
    IGpio& gpio_;
};

} // namespace io
} // namespace ocs
