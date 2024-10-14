/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "driver/gpio.h"
#include "driver/i2c_master.h"

#include "ocs_core/noncopyable.h"
#include "ocs_i2c/istore.h"

namespace ocs {
namespace i2c {

class MasterStore : public IStore, public core::NonCopyable<> {
public:
    struct Params {
        //! Data line.
        gpio_num_t sda { GPIO_NUM_NC };

        //! Clock line.
        gpio_num_t scl { GPIO_NUM_NC };
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
