/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/bit_ops.h"
#include "ocs_onewire/rom_code_scanner.h"
#include "ocs_onewire/serial_number_to_str.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace onewire {

RomCodeScanner::RomCodeScanner(Bus& bus)
    : bus_(bus) {
}

status::StatusCode RomCodeScanner::scan(RomCode& rom_code) {
    if (finished_) {
        return status::StatusCode::NoData;
    }

    OCS_STATUS_RETURN_ON_ERROR(bus_.reset());

    OCS_STATUS_RETURN_ON_ERROR(
        bus_.write_byte(static_cast<uint8_t>(RomCode::Command::SearchRom)));

    OCS_STATUS_RETURN_ON_ERROR(
        scan_(reinterpret_cast<uint8_t*>(&rom_code), sizeof(rom_code)));

    OCS_STATUS_RETURN_ON_FALSE(rom_code.valid(), status::StatusCode::Error);

    if (!conflict_count_) {
        finished_ = true;
    }

    return status::StatusCode::OK;
}

void RomCodeScanner::reset() {
    finished_ = false;
    prev_discrepancy_ = -1;
    curr_discrepancy_ = -1;
    conflict_count_ = 0;
}

status::StatusCode RomCodeScanner::scan_(uint8_t* buf, unsigned size) {
    for (int n = 0; n < size * bits_in_byte_; ++n) {
        uint8_t bit1 = 0;
        OCS_STATUS_RETURN_ON_ERROR(bus_.read_bit(bit1));

        uint8_t bit2 = 0;
        OCS_STATUS_RETURN_ON_ERROR(bus_.read_bit(bit2));

        if (bit1 && bit2) {
            return status::StatusCode::NoData;
        }

        uint8_t bit = 0;

        if (!bit1 && !bit2) {
            bit = handle_discrepancy_(buf, n);
        } else {
            if (bit1 && !bit2) {
                bit = 1;
            } else {
                bit = 0;
            }
        }

        const unsigned byte_pos = n / bits_in_byte_;
        const unsigned bit_pos = n % bits_in_byte_;

        if (bit) {
            buf[byte_pos] |= algo::BitOps::mask(bit_pos);
        } else {
            buf[byte_pos] &= algo::BitOps::umask(bit_pos);
        }

        OCS_STATUS_RETURN_ON_ERROR(bus_.write_bit(bit));
    }

    return status::StatusCode::OK;
}

uint8_t RomCodeScanner::handle_discrepancy_(const uint8_t* buf, int position) {
    configASSERT(buf);

    if (curr_discrepancy_ == position) {
        // Current discrepancy is handled, handle the previous one.
        if (!position) {
            prev_discrepancy_ = 0;
            curr_discrepancy_ = -1;

            conflict_count_ = 0;
        } else {
            curr_discrepancy_ = prev_discrepancy_;

            configASSERT(conflict_count_);
            --conflict_count_;
        }

        return 1;
    }

    if (position > curr_discrepancy_) {
        // New discrepancy is found.
        if (prev_discrepancy_ < 0) {
            prev_discrepancy_ = position;
            curr_discrepancy_ = position;
        } else {
            prev_discrepancy_ = curr_discrepancy_;
            curr_discrepancy_ = position;
        }

        ++conflict_count_;

        return 0;
    }

    // Follow to the previous path.
    configASSERT(prev_discrepancy_ >= 0);

    const unsigned byte_pos = prev_discrepancy_ / bits_in_byte_;
    const unsigned bit_pos = prev_discrepancy_ % bits_in_byte_;

    prev_discrepancy_ = position;

    return algo::BitOps::nth(buf[byte_pos], bit_pos);
}

} // namespace onewire
} // namespace ocs
