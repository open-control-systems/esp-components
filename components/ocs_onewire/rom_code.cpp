/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "ocs_onewire/crc.h"
#include "ocs_onewire/rom_code.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace onewire {

RomCode::RomCode() {
    memset(serial_number, 0, sizeof(serial_number));
}

bool RomCode::valid() const {
    const auto calculated_crc = calculate_crc(reinterpret_cast<const uint8_t*>(this),
                                              sizeof(RomCode) - sizeof(crc));
    if (calculated_crc != crc) {
        return false;
    }

    return true;
}

status::StatusCode RomCode::match(onewire::Bus& bus) const {
    OCS_STATUS_RETURN_ON_ERROR(bus.reset());

    OCS_STATUS_RETURN_ON_ERROR(bus.write_byte(static_cast<uint8_t>(Commands::MatchRom)));

    OCS_STATUS_RETURN_ON_ERROR(
        bus.write_bytes(reinterpret_cast<const uint8_t*>(this), sizeof(RomCode)));

    return status::StatusCode::OK;
}

status::StatusCode RomCode::read(onewire::Bus& bus) {
    OCS_STATUS_RETURN_ON_ERROR(bus.reset());

    OCS_STATUS_RETURN_ON_ERROR(bus.write_byte(static_cast<uint8_t>(Commands::ReadRom)));

    OCS_STATUS_RETURN_ON_ERROR(
        bus.read_bytes(reinterpret_cast<uint8_t*>(this), sizeof(RomCode)));

    return status::StatusCode::OK;
}

} // namespace onewire
} // namespace ocs
