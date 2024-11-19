/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_algo/sha_engine_ops.h"
#include "ocs_core/noncopyable.h"
#include "ocs_security/isha_engine.h"

namespace ocs {
namespace security {

class sha_to_hex_str : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p sha - SHA data, @p size bytes long.
    sha_to_hex_str(const uint8_t* sha, unsigned size);

    //! Return SHA formatted as a hex string.
    const char* c_str() const;

private:
    static const constexpr unsigned max_hash_length_ =
        algo::ShaEngineOps::hash_lenght(IShaEngine::Algorithm::SHA512);

    char buf_[max_hash_length_ + 1];
};

} // namespace security
} // namespace ocs
