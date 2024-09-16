/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/ds18b20/resolution_to_str.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

const char* resolution_to_str(Sensor::Configuration::Resolution resolution) {
    switch (resolution) {
    case Sensor::Configuration::Resolution::Bit_12:
        return "Bit_12";
    case Sensor::Configuration::Resolution::Bit_11:
        return "Bit_11";
    case Sensor::Configuration::Resolution::Bit_10:
        return "Bit_10";
    case Sensor::Configuration::Resolution::Bit_9:
        return "Bit_9";

    default:
        break;
    }

    return "<none>";
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
