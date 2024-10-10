/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>

#include "driver/i2c_master.h"

#include "ocs_core/noncopyable.h"
#include "ocs_i2c/itransmitter.h"

namespace ocs {
namespace i2c {

class MasterTransmitter : public ITransmitter, public core::NonCopyable<> {
public:
    //! Initialize
    //!
    //! @params
    //!  - @p handle - I2C device handle.
    //!  - @p id to distinguish one transmitter from another.
    MasterTransmitter(i2c_master_dev_handle_t handle, const char* id);

    //! Transmit data to the I2C device.
    status::StatusCode transmit(const uint8_t* buf,
                                unsigned size,
                                core::microseconds_t timeout = -1) override;

    //! Receive data from the I2C device.
    status::StatusCode
    receive(uint8_t* buf, unsigned size, core::microseconds_t timeout = -1) override;

private:
    const std::string id_;

    i2c_master_dev_handle_t handle_ { nullptr };
};

} // namespace i2c
} // namespace ocs
