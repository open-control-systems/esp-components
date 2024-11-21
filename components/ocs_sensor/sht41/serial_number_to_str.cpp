/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstdio>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/sht41/serial_number_to_str.h"

namespace ocs {
namespace sensor {
namespace sht41 {

serial_number_to_str::serial_number_to_str(const SerialNumber& serial_number) {
    memset(buf_, 0, sizeof(buf_));

    configASSERT(snprintf(buf_, sizeof(buf_), SHT_SENSOR_SERIAL_NUMBER_STR,
                          SHT_SENSOR_SERIAL_NUMBER_TO_STR(serial_number))
                 == str_length);
}

const char* serial_number_to_str::c_str() const {
    return buf_;
}

} // namespace sht41
} // namespace sensor
} // namespace ocs
