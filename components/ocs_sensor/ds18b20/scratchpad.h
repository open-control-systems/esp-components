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
#include "ocs_onewire/rom_code.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

struct OCS_ATTR_PACKED Scratchpad {
    enum class Command : uint8_t {
        ConvertT = 0x44,
        ReadScratchpad = 0xBE,
        WriteScratchpad = 0x4E,
        CopyScratchpad = 0x48,
        RecallE2 = 0xB8,
        ReadPowerSupply = 0xB4,
    };
    //! Return true if the scratchpad contains the valid data.
    bool valid() const;

    //! Return sensor temperature in Celsius based on the configured resolution.
    float get_temperature() const;

    //! Write scratchpad to the bus.
    status::StatusCode write(onewire::Bus& bus, const onewire::RomCode& rom_code) const;

    //! Read scratchpad from the bus.
    status::StatusCode read(onewire::Bus& bus, const onewire::RomCode& rom_code);

    //! Read-only data.
    uint8_t temperature_lsb { 0 };
    uint8_t temperature_msb { 0 };

    //! High and low alarm trigger registers.
    //!
    //! @remarks
    //!  - Can be saved in EEPROM.
    //!
    //!  - If the DS18B20 alarm function is not used, the TH and TL registers can
    //!    serve as general-purpose memory.
    uint8_t alarm_trigger_high { 0 };
    uint8_t alarm_trigger_low { 0 };

    //! Can be saved in EEPROM.
    struct OCS_ATTR_PACKED Configuration {
        uint8_t reserved_0 : 1 { 0 };
        uint8_t reserved_1 : 1 { 0 };
        uint8_t reserved_2 : 1 { 0 };
        uint8_t reserved_3 : 1 { 0 };
        uint8_t reserved_4 : 1 { 0 };

        //! | R0  | R1  | Resolution Bits | Max Conversation Time |
        //! | --- | --- | --------------- | --------------------- |
        //! | 1   | 1   | 12              | 750.0ms (tconv)       |
        //! | 0   | 1   | 11              | 375.0ms (tconv / 2)   |
        //! | 1   | 0   | 10              | 187.5ms (tconv / 4)   |
        //! | 0   | 0   | 9               | 93.75ms (tconv / 8)   |
        uint8_t r0 : 1 { 0 };
        uint8_t r1 : 1 { 0 };

        uint8_t reserved_7 : 1 { 0 };
    };

    Configuration configuration;
    static_assert(sizeof(configuration) == 1, "Configuration register size mismatch");

    uint8_t reserved_5 { 0 };
    uint8_t reserved_6 { 0 };
    uint8_t reserved_7 { 0 };

    //! Read-only data.
    uint8_t crc { 0 };
};

static_assert(sizeof(Scratchpad) == 9, "Scratchpad size mismatch");

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
