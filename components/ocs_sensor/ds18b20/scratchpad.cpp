/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>
#include <cstring>

#include "ocs_core/bit_ops.h"
#include "ocs_onewire/crc.h"
#include "ocs_sensor/ds18b20/scratchpad.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

bool Scratchpad::valid() const {
    const auto calculated_crc = onewire::calculate_crc(
        reinterpret_cast<const uint8_t*>(this), sizeof(Scratchpad) - sizeof(crc));
    if (calculated_crc != crc) {
        return false;
    }

    return true;
}

float Scratchpad::get_temperature() const {
    float result = 0;

    if (configuration.r0 == 1 && configuration.r1 == 1) {
        // 12-bits.
        if (core::BitOps::nth(temperature_lsb, 0)) {
            result += std::pow(2, -4);
        }
        if (core::BitOps::nth(temperature_lsb, 1)) {
            result += std::pow(2, -3);
        }
        if (core::BitOps::nth(temperature_lsb, 2)) {
            result += std::pow(2, -2);
        }
        if (core::BitOps::nth(temperature_lsb, 3)) {
            result += std::pow(2, -1);
        }
    } else if (configuration.r0 == 0 && configuration.r1 == 1) {
        // 11-bits
        if (core::BitOps::nth(temperature_lsb, 1)) {
            result += std::pow(2, -3);
        }
        if (core::BitOps::nth(temperature_lsb, 2)) {
            result += std::pow(2, -2);
        }
        if (core::BitOps::nth(temperature_lsb, 3)) {
            result += std::pow(2, -1);
        }
    } else if (configuration.r0 == 1 && configuration.r1 == 0) {
        // 10-bits
        if (core::BitOps::nth(temperature_lsb, 2)) {
            result += std::pow(2, -2);
        }
        if (core::BitOps::nth(temperature_lsb, 3)) {
            result += std::pow(2, -1);
        }
    } else {
        // 9-bits
        if (core::BitOps::nth(temperature_lsb, 3)) {
            result += std::pow(2, -1);
        }
    }

    if (core::BitOps::nth(temperature_lsb, 4)) {
        result += std::pow(2, 0);
    }
    if (core::BitOps::nth(temperature_lsb, 5)) {
        result += std::pow(2, 1);
    }
    if (core::BitOps::nth(temperature_lsb, 6)) {
        result += std::pow(2, 2);
    }
    if (core::BitOps::nth(temperature_lsb, 7)) {
        result += std::pow(2, 3);
    }

    if (core::BitOps::nth(temperature_msb, 0)) {
        result += std::pow(2, 4);
    }
    if (core::BitOps::nth(temperature_msb, 1)) {
        result += std::pow(2, 5);
    }
    if (core::BitOps::nth(temperature_msb, 2)) {
        result += std::pow(2, 6);
    }

    const uint8_t sign_mask = 0b11111000;
    if ((temperature_msb & sign_mask) == sign_mask) {
        result = -result;
    }

    return result;
}

status::StatusCode Scratchpad::write(onewire::Bus& bus,
                                     const onewire::RomCode& rom_code) const {
    OCS_STATUS_RETURN_ON_ERROR(rom_code.match(bus));

    OCS_STATUS_RETURN_ON_ERROR(
        bus.write_byte(static_cast<uint8_t>(Command::WriteScratchpad)));

    // Only bytes 2, 3, 4 of the scratchpad can be modified.
    OCS_STATUS_RETURN_ON_ERROR(
        bus.write_bytes(reinterpret_cast<const uint8_t*>(this) + 2, 3));

    return status::StatusCode::OK;
}

status::StatusCode Scratchpad::read(onewire::Bus& bus, const onewire::RomCode& rom_code) {
    OCS_STATUS_RETURN_ON_ERROR(rom_code.match(bus));

    OCS_STATUS_RETURN_ON_ERROR(
        bus.write_byte(static_cast<uint8_t>(Command::ReadScratchpad)));

    OCS_STATUS_RETURN_ON_ERROR(
        bus.read_bytes(reinterpret_cast<uint8_t*>(this), sizeof(Scratchpad)));

    return status::StatusCode::OK;
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
