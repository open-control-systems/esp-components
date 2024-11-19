/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_security/basic_sha_engine.h"

namespace ocs {
namespace security {

class MbedTlsShaEngine : public BasicShaEngine, public core::NonCopyable<> {
public:
    //! Initialize.
    explicit MbedTlsShaEngine(IShaEngine::Algorithm algorithm);

    //! Generate SHA by using the mbedTLS library.
    status::StatusCode generate(uint8_t* buf, const uint8_t* src, unsigned size) override;
};

} // namespace security
} // namespace ocs
