/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/macros.h"
#include "ocs_onewire/bus.h"

#define DS_SENSOR_SERIAL_NUMBER_LEN 6

#define DS_SENSOR_SERIAL_NUMBER_STR "%02X:%02X:%02X:%02X:%02X:%02X"

#define DS_SENSOR_SERIAL_NUMBER_TO_STR(serial_number)                                    \
    serial_number[0], serial_number[1], serial_number[2], serial_number[3],              \
        serial_number[4], serial_number[5]

namespace ocs {
namespace onewire {

using SerialNumber = uint8_t[DS_SENSOR_SERIAL_NUMBER_LEN];

struct OCS_ATTR_PACKED RomCode {
    enum class Command : uint8_t {
        SearchRom = 0xF0,
        ReadRom = 0x33,
        MatchRom = 0x55,
        SkipRom = 0xCC,
        AlarmSearch = 0xEC,
    };

    //! Initialize empty rom code.
    RomCode();

    //! Return true if the rom code contains the valid data.
    bool valid() const;

    //! Match rom code on the bus.
    status::StatusCode match(Bus& bus) const;

    //! Read rom code from the bus.
    status::StatusCode read(Bus& bus);

    //! Known family codes:
    //!  - DS18B20: 0x28.
    uint8_t family_code { 0 };
    SerialNumber serial_number;
    uint8_t crc { 0 };
};

static_assert(sizeof(RomCode) == 8, "Rom code size mismatch");

} // namespace onewire
} // namespace ocs
