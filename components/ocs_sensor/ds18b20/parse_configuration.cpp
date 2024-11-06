/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/ds18b20/parse_configuration.h"
#include "ocs_algo/string_ops.h"
#include "ocs_core/macros.h"
#include "ocs_onewire/rom_code.h"
#include "ocs_onewire/serial_number_to_str.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

namespace {

Sensor::Configuration::Resolution parse_resolution(const std::string_view& str) {
    if (str == "12") {
        return Sensor::Configuration::Resolution::Bit_12;
    }
    if (str == "11") {
        return Sensor::Configuration::Resolution::Bit_11;
    }
    if (str == "10") {
        return Sensor::Configuration::Resolution::Bit_10;
    }
    if (str == "9") {
        return Sensor::Configuration::Resolution::Bit_9;
    }

    return Sensor::Configuration::Resolution::None;
}

uint8_t parse_hex(char hi, char lo) {
    const int h = isdigit(hi) ? hi - '0' : tolower(hi) - 'a' + 10;
    const int l = isdigit(lo) ? lo - '0' : tolower(lo) - 'a' + 10;
    return static_cast<uint8_t>(h << 4 | l);
}

bool parse_serial_number(onewire::SerialNumber& serial_number,
                         const std::string_view& str) {
    if (str.size() != onewire::serial_number_to_str::str_length) {
        return false;
    }

    const auto values = algo::StringOps::split(':', str);
    if (values.size() != OCS_ARRAY_SIZE(serial_number)) {
        return false;
    }

    for (unsigned n = 0; n < OCS_ARRAY_SIZE(serial_number); ++n) {
        const auto str = values[n];
        if (str.size() != strlen("AA")) {
            return false;
        }

        serial_number[n] = parse_hex(str[0], str[1]);
    }

    return true;
}

} // namespace

status::StatusCode parse_configuration(Sensor::Configuration& configuration,
                                       const std::string_view& serial_number,
                                       const std::string_view& resolution) {
    if (!parse_serial_number(configuration.rom_code.serial_number, serial_number)) {
        return status::StatusCode::InvalidArg;
    }

    configuration.resolution = parse_resolution(resolution);
    if (configuration.resolution == Sensor::Configuration::Resolution::None) {
        return status::StatusCode::InvalidArg;
    }

    return status::StatusCode::OK;
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
