/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/igpio.h"

namespace ocs {
namespace io {

class GpioGuard : public core::NonCopyable<> {
public:
    //! Turn on @p gpio on initialization.
    explicit GpioGuard(IGpio& gpio);

    //! Turn off gpio on de-initialization.
    ~GpioGuard();

private:
    IGpio& gpio_;
};

} // namespace io
} // namespace ocs
