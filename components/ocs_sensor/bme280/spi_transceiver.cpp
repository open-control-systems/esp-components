/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "ocs_core/bit_ops.h"
#include "ocs_sensor/bme280/spi_transceiver.h"

namespace ocs {
namespace sensor {
namespace bme280 {

SpiTransceiver::SpiTransceiver(spi::ITransceiver& transceiver)
    : transceiver_(transceiver) {
}

status::StatusCode
SpiTransceiver::send(const uint8_t* buf, unsigned size, RegisterAddress addr) {
    //! Each register is sent as a pair of bytes: | control byte | data byte |.
    const auto send_buf_size = size * 2;
    uint8_t send_buf[send_buf_size];

    unsigned pos = 0;

    for (unsigned n = 0; n < send_buf_size; n += 2) {
        //! Set the MSB bit to 0 to indicate a write command.
        const uint8_t control_byte = addr & core::BitOps::umask(8);
        const uint8_t data_byte = buf[pos];

        send_buf[n] = control_byte;
        send_buf[n + 1] = data_byte;

        ++pos;
        ++addr;
    }

    return transceiver_.transceive(send_buf, sizeof(send_buf), nullptr, 0);
}

status::StatusCode
SpiTransceiver::receive(uint8_t* buf, unsigned size, RegisterAddress addr) {
    //! Set the MSB bit to 1 to indicate a read command.
    const uint8_t control_byte = addr | core::BitOps::mask(8);

    const uint8_t send_buf[] = { control_byte };

    return transceiver_.transceive(send_buf, sizeof(send_buf), buf, size);
}

} // namespace bme280
} // namespace sensor
} // namespace ocs
