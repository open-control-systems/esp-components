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

namespace ocs {
namespace diagnostic {

class BasicCounter : public ICounter, public core::NonCopyable<BasicCounter> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p id - counter identifier, 15 characters is a maximum length.
    explicit BasicCounter(const char* id);

    //! Destroy.
    virtual ~BasicCounter() = default;

    //! Return the underlying counter identifier.
    const char* id() const;

private:
    static const constexpr unsigned bufsize_ = 15;

    char buf_[bufsize_ + 1];
};

} // namespace diagnostic
} // namespace ocs
