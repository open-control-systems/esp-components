/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "driver/i2c_master.h"

#include "ocs_core/noncopyable.h"
#include "ocs_i2c/istore.h"
#include "ocs_io/gpio/types.h"

namespace ocs {
namespace i2c {

class MasterStore : public IStore, public core::NonCopyable<> {
public:
    struct Params {
        //! Data line.
        io::gpio::Gpio sda { static_cast<io::gpio::Gpio>(-1) };

        //! Clock line.
        io::gpio::Gpio scl { static_cast<io::gpio::Gpio>(-1) };
    };

    //! Initialize I2C master bus.
    explicit MasterStore(Params params);

    //! De-initialize I2C master bus.
    ~MasterStore();

    //! Add I2C slave device to the store.
    IStore::ITransceiverPtr
    add(const char* id, AddressLength len, Address addr, TransferSpeed speed) override;

private:
    i2c_master_bus_handle_t handle_ { nullptr };
};

} // namespace i2c
} // namespace ocs
