/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_i2c/itransmitter.h"

namespace ocs {
namespace i2c {

class IStore {
public:
    enum class TransferSpeed : uint8_t {
        //! 100 kbit/s.
        Default,

        //! 400 kbit/s.
        Fast,
    };

    enum class AddressLen : uint8_t {
        //! 7-bit I2C address.
        Bit_7,

        //! 10-bit I2C address.
        Bit_10,
    };

    using DeviceAddress = uint16_t;

    //! Destroy.
    virtual ~IStore() = default;

    //! Register a new I2C device.
    //!
    //! @params
    //!  - @p address_length - I2C device address length.
    //!  - @p address - actual I2C device address.
    //!  - @p speed - how fast data will be transmitted over I2C bus.
    ITransmitterPtr
    add(AddressLen address_length, DeviceAddress address, TransferSpeed speed);
};

} // namespace i2c
} // namespace ocs
