/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "driver/spi_master.h"

namespace ocs {
namespace spi {

//! SPI mode, representing a pair of (CPOL, CPHA) configuration:
//! - 0: (0, 0)
//! - 1: (0, 1)
//! - 2: (1, 0)
//! - 3: (1, 1)
//!
//! @references
//!  https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
//!  https://www.analog.com/en/resources/analog-dialogue/articles/introduction-to-spi-interface.html
using Mode = uint8_t;

//! SPI peripheral unique identifier.
using HostID = uint8_t;

//! SPI transfer speed, in hertz.
using TransferSpeed = int;

//! Virtual SPI peripheral.
const HostID VSPI = SPI3_HOST;

} // namespace spi
} // namespace ocs
