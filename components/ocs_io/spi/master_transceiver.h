/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <string>

#include "driver/spi_master.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/spi/itransceiver.h"

namespace ocs {
namespace io {
namespace spi {

class MasterTransceiver : public ITransceiver, public core::NonCopyable<> {
public:
    using DevicePtr = std::shared_ptr<spi_device_t>;
    static DevicePtr make_device_shared(spi_device_t* device);

    //! Initialize
    //!
    //! @params
    //!  - @p device - SPI device.
    //!  - @p id to distinguish one transceiver from another.
    MasterTransceiver(DevicePtr device, const char* id);

    //! Send/receive data over SPI bus.
    status::StatusCode transceive(const uint8_t* send_buf,
                                  unsigned send_buf_size,
                                  uint8_t* recv_buf,
                                  unsigned recv_buf_size) override;

private:
    const std::string id_;

    DevicePtr device_;
};

} // namespace spi
} // namespace io
} // namespace ocs
