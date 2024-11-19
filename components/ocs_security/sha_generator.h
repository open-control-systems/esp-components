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
#include "ocs_security/isha_engine.h"

namespace ocs {
namespace security {

class ShaGenerator : public core::NonCopyable<> {
public:
    using Data = std::vector<uint8_t>;

    //! Initialize.
    explicit ShaGenerator(IShaEngine& engine);

    //! Return the result of SHA calculation.
    const Data& get_sha() const;

    //! Add @p size bytes from @p buf to be used during SHA calculation.
    void add(const uint8_t* buf, unsigned size);

    //! Generate SHA based on the underlying data.
    status::StatusCode generate();

private:
    IShaEngine& engine_;

    Data src_;
    Data sha_;
};

} // namespace security
} // namespace ocs
