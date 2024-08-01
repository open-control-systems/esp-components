/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_io/default_gpio.h"

namespace ocs {
namespace io {

DefaultGpio::DefaultGpio(const char* id, gpio_num_t gpio)
    : BasicGpio(id, gpio, true) {
}

} // namespace io
} // namespace ocs
