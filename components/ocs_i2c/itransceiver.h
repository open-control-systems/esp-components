/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/time.h"
#include "ocs_status/code.h"

namespace ocs {
namespace i2c {

class ITransceiver {
public:
    //! Destroy.
    virtual ~ITransceiver() = default;

    //! Send data to the I2C device.
    //!
    //! @params
    //!  - @p buf - sending data, should be at least @p size bytes long.
    //!  - @p timeout - interval to wait for the operation to complete,
    //!       -1 means wait forever.
    virtual status::StatusCode
    send(const uint8_t* buf, unsigned size, core::microseconds_t timeout) = 0;

    //! Receive data from the I2C device.
    //!
    //! @params
    //!  - @p buf - buffer to store received data, should be at least @p size bytes long.
    //!  - @p timeout - interval to wait for the operation to complete,
    //!       -1 means wait forever.
    virtual status::StatusCode
    receive(uint8_t* buf, unsigned size, core::microseconds_t timeout) = 0;
};

} // namespace i2c
} // namespace ocs
