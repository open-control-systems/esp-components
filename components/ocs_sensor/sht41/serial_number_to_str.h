/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstring>

#include "ocs_core/noncopyable.h"
#include "ocs_sensor/sht41/serial_number.h"

namespace ocs {
namespace sensor {
namespace sht41 {

//! Format sensor serial number to string.
class serial_number_to_str : public core::NonCopyable<> {
public:
    static const unsigned str_length = strlen("AA:BB:CC:DD");

    //! Initialize.
    serial_number_to_str(const SerialNumber& serial_number);

    //! Return formatted serial number.
    const char* c_str() const;

private:
    char buf_[str_length + 1];
};

} // namespace sht41
} // namespace sensor
} // namespace ocs
