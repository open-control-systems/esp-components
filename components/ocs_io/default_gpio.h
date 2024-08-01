/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/basic_gpio.h"

namespace ocs {
namespace io {

//! High logic level is used to enable GPIO.
class DefaultGpio : public BasicGpio, public core::NonCopyable<> {
public:
    //! Initialize.
    DefaultGpio(const char* id, gpio_num_t gpio);
};

} // namespace io
} // namespace ocs
