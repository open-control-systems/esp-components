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
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace scheduler {

class FanoutTask : public ITask, public core::NonCopyable<> {
public:
    //! Propagate call to the underlying tasks.
    status::StatusCode run() override;

    //! Add task to be invoked on a run() call.
    void add(ITask& task);

private:
    std::vector<ITask*> tasks_;
};

} // namespace scheduler
} // namespace ocs
