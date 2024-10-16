/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace spi {

class ITransceiver {
public:
    //! Destroy.
    virtual ~ITransceiver() = default;

    //! SPI register address.
    using Address = uint64_t;

    //! Send data to the SPI device.
    //!
    //! @params
    //!  - @p buf - sending data, should be at least @p size bytes long.
    //!  - @p addr - SPI register address from where to write @p size bytes from @p buf.
    virtual status::StatusCode send(const uint8_t* buf, unsigned size, Address addr) = 0;

    //! Receive data from the SPI device.
    //!
    //! @params
    //!  - @p buf - buffer to store received data, should be at least @p size bytes long.
    //!  - @p addr - SPI register address from where to read @p size bytes to @p buf.
    virtual status::StatusCode receive(uint8_t* buf, unsigned size, Address addr) = 0;
};

} // namespace spi
} // namespace ocs
