/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/basic_persistent_counter.h"

namespace ocs {
namespace diagnostic {

class MemPersistentCounter : public BasicPersistentCounter, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p storage to persist counter values when reboot is happened.
    //!  - @p counter to handle actual counting value.
    MemPersistentCounter(storage::IStorage& storage, ICounter& counter);

    //! Returns the accumulated counter value, considering the previous persistent value.
    //!
    //! @remarks
    //!  If a power cut is happened, the persisted value is lost.
    Value get() const override;
};

} // namespace diagnostic
} // namespace ocs
