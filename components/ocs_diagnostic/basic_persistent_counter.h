/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/icounter.h"
#include "ocs_scheduler/itask.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/ireboot_handler.h"

namespace ocs {
namespace diagnostic {

class BasicPersistentCounter : public ICounter,
                               public system::IRebootHandler,
                               public scheduler::ITask,
                               public core::NonCopyable<BasicPersistentCounter> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p storage to persist counter values when reboot is happened.
    //!  - @p counter to handle actual counting value.
    //!
    //! @remarks
    //!  All operations should be scheduled on the same task scheduler.
    BasicPersistentCounter(storage::IStorage& storage, ICounter& counter);

    //! Destroy.
    virtual ~BasicPersistentCounter() = default;

    //! Return the counter identifier.
    const char* id() const override;

    //! Persist the counter value on reboot.
    void handle_reboot() override;

    //! Save counter value in a persistent storage.
    status::StatusCode run() override;

protected:
    ICounter& counter_;

    ICounter::Value value_ { 0 };

private:
    status::StatusCode save_();

    storage::IStorage& storage_;
};

} // namespace diagnostic
} // namespace ocs
