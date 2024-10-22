/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_sensor/bme280/itransceiver.h"
#include "ocs_spi/itransceiver.h"

namespace ocs {
namespace sensor {
namespace bme280 {

class SpiTransceiver : public ITransceiver, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p transceiver to communicate with SPI device.
    explicit SpiTransceiver(spi::ITransceiver& transceiver);

    //! Send registers to SPI device.
    status::StatusCode
    send(const uint8_t* buf, unsigned size, RegisterAddress addr) override;

    //! Receive registers from SPI device.
    status::StatusCode
    receive(uint8_t* buf, unsigned size, RegisterAddress addr) override;

private:
    spi::ITransceiver& transceiver_;
};

} // namespace bme280
} // namespace sensor
} // namespace ocs
