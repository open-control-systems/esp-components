/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstring>

#include "ocs_core/noncopyable.h"
#include "ocs_core/version.h"

namespace ocs {
namespace core {

class version_to_str : public NonCopyable<> {
public:
    //! Initialize.
    explicit version_to_str(Version version);

    //! Return formatted version.
    const char* c_str() const;

private:
    static const unsigned size_ = strlen("65535.65535.65535");

    char buf_[size_ + 1];
};

} // namespace core
} // namespace ocs
