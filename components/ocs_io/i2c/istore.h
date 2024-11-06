/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_io/i2c/itransceiver.h"

namespace ocs {
namespace io {
namespace i2c {

class IStore {
public:
    enum class TransferSpeed : uint8_t {
        //! 100 kbit/s.
        Default,

        //! 400 kbit/s.
        Fast,
    };

    enum class AddressLength : uint8_t {
        //! 7-bit I2C address.
        Bit_7,

        //! 10-bit I2C address.
        Bit_10,
    };

    //! I2C address.
    using Address = uint16_t;

    //! I2C transceiver to communicate with I2C device.
    using ITransceiverPtr = std::unique_ptr<ITransceiver>;

    //! Destroy.
    virtual ~IStore() = default;

    //! Register a new I2C device.
    //!
    //! @params
    //!  - @p id - to distinguish one I2C device from another.
    //!  - @p len - I2C device address length.
    //!  - @p addr - actual I2C device address.
    //!  - @p speed - how fast data will be transmitted over I2C bus.
    virtual ITransceiverPtr
    add(const char* id, AddressLength len, Address addr, TransferSpeed speed) = 0;
};

} // namespace i2c
} // namespace io
} // namespace ocs
