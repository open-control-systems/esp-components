/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "driver/gpio.h"

#include "ocs_core/noncopyable.h"
#include "ocs_spi/istore.h"

namespace ocs {
namespace spi {

class MasterStore : public IStore, public core::NonCopyable<> {
public:
    struct Params {
        //! MOSI line.
        gpio_num_t mosi { GPIO_NUM_NC };

        //! MISO line.
        gpio_num_t miso { GPIO_NUM_NC };

        //! SCLK line.
        gpio_num_t sclk { GPIO_NUM_NC };

        //! Maximum number of bytes transferred over the SPI line in a single transaction.
        int max_transfer_size { 0 };

        //! SPI peripheral ID.
        HostID host_id { 0 };
    };

    //! Initialize SPI master bus.
    explicit MasterStore(Params params);

    //! De-initialize SPI master bus.
    ~MasterStore();

    //! Add SPI slave device to the store.
    IStore::ITransceiverPtr
    add(const char* id, gpio_num_t cs, Mode mode, TransferSpeed speed) override;

private:
    const Params params_;
};

} // namespace spi
} // namespace ocs
