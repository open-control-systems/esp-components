/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_security/isha_engine.h"

namespace ocs {
namespace security {

class BasicShaEngine : public IShaEngine, public core::NonCopyable<BasicShaEngine> {
public:
    //! Destroy.
    virtual ~BasicShaEngine() = default;

    //! Initialize.
    explicit BasicShaEngine(IShaEngine::Algorithm algorithm);

    //! Return selected SHA algorithm.
    IShaEngine::Algorithm get_algorithm() const override;

protected:
    const IShaEngine::Algorithm algorithm_ { IShaEngine::Algorithm::SHA1 };
};

} // namespace security
} // namespace ocs
