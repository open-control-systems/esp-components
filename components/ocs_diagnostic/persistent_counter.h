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
#include "ocs_storage/istorage.h"
#include "ocs_system/ireboot_handler.h"

namespace ocs {
namespace diagnostic {

class PersistentCounter : public ICounter,
                          public system::IRebootHandler,
                          public core::NonCopyable<PersistentCounter> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p storage to persist counter values when reboot is happened.
    //!  - @p counter to handle actual counting value.
    PersistentCounter(storage::IStorage& storage, ICounter& counter);

    //! Destroy.
    virtual ~PersistentCounter() = default;

    //! Return the counter identifier.
    const char* id() const override;

    //! Return the current counter value, taking into account previously stored value.
    ICounter::Value get() const override;

    //! Persist the counter value on reboot.
    void handle_reboot() override;

private:
    storage::IStorage& storage_;
    ICounter& counter_;

    ICounter::Value value_ { 0 };
};

} // namespace diagnostic
} // namespace ocs
