/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/persistent_counter.h"

namespace ocs {
namespace diagnostic {

//! Monitor the counter value while the system is in operation, taking into account the
//! reboot events. If a power cut is happened, the persisted value is lost.
class LiveCounter : public PersistentCounter, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p storage to persist counter values when reboot is happened.
    //!  - @p counter to handle actual counting value.
    LiveCounter(storage::IStorage& storage, ICounter& counter);
};

} // namespace diagnostic
} // namespace ocs
