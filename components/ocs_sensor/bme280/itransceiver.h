/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_sensor/bme280/protocol.h"
#include "ocs_status/code.h"

namespace ocs {
namespace sensor {
namespace bme280 {

class ITransceiver {
public:
    //! Destroy.
    virtual ~ITransceiver() = default;

    //! Send @p size registers from @p buf start with @p addr.
    virtual status::StatusCode
    send(const uint8_t* buf, unsigned size, RegisterAddress addr) = 0;

    //! Receive @p size registers to @p buf start with @p addr.
    virtual status::StatusCode
    receive(uint8_t* buf, unsigned size, RegisterAddress addr) = 0;
};

} // namespace bme280
} // namespace sensor
} // namespace ocs
