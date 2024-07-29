/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/iclock.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/time.h"
#include "ocs_diagnostic/basic_counter.h"

namespace ocs {
namespace diagnostic {

class TimeCounter : public BasicCounter, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p clock to read a time since boot.
    //!  - @p id - counter identifier.
    //!  - @p resolution - time resolution used to count the value.
    TimeCounter(core::IClock& clock, const char* id, core::microseconds_t resolution);

    //! Return time since boot with the configured resolution.
    ICounter::Value get() const override;

    //! Start counting time from @p now.
    void reset(core::microseconds_t now);

private:
    const core::microseconds_t resolution_ = core::Microsecond;

    core::IClock& clock_;

    core::microseconds_t offset_ { 0 };
};

} // namespace diagnostic
} // namespace ocs
