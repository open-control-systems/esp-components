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
#include "ocs_status/macros.h"

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
    memset(send_buf, 0, sizeof(send_buf));

    unsigned pos = 0;

    for (unsigned n = 0; n < send_buf_size;) {
        //! Set the MSB bit to 0 to indicate a write command.
        const uint8_t control_byte = addr & core::BitOps::umask(7);
        const uint8_t data_byte = buf[pos];

        send_buf[n++] = control_byte;
        send_buf[n++] = data_byte;

        ++pos;
        ++addr;
    }

    return transceiver_.transceive(send_buf, sizeof(send_buf), nullptr, 0);
}

status::StatusCode
SpiTransceiver::receive(uint8_t* buf, unsigned size, RegisterAddress addr) {
    //! Dummy bytes are used to clock out the data we want to receive.
    uint8_t send_buf[size + 1];
    memset(send_buf, 0, sizeof(send_buf));

    //! Set the MSB bit to 1 to indicate a read command.
    send_buf[0] = addr | core::BitOps::mask(7);

    uint8_t recv_buf[size + 1];
    memset(recv_buf, 0, sizeof(recv_buf));

    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_.transceive(send_buf, sizeof(send_buf), recv_buf, sizeof(recv_buf)));

    memcpy(buf, recv_buf + 1, size);

    return status::StatusCode::OK;
}

} // namespace bme280
} // namespace sensor
} // namespace ocs
