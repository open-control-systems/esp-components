/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/igpio.h"
#include "ocs_status/code.h"
#include "ocs_system/idelayer.h"

namespace ocs {
namespace onewire {

//! The 1-Wire bus system uses a single bus master to control one or more slave devices.
//!
//! @notes
//!  Key protocol considerations:
//!   - Master should follow the protocol transaction sequence:
//!      - Initialization.
//!      - Rom command, followed by any required data exchange.
//!      - Sensor Function Command, followed by any required data exchange.
//!   - All data and commands are transmitted least significant bit first.
//!   - One bit of data is transmitted over the bus per a single time slot.
//!   - All write time slots should be at least of 60 microseconds.
//!   - A recovery delay of at least 1 microsecond is used after each write time slot.
//!   - All read time slots should be at least of 60 microseconds.
//!   - A recovery delay of at least 1 microsecond is used after each read time slot.
//!
//! @reference
//!  https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf
//!  https://pdfserv.maximintegrated.com/en/an/AN937.pdf
//!  https://www.analog.com/en/resources/technical-articles/1wire-communication-through-software.html
class Bus : public core::NonCopyable<> {
public:
    struct Params {
        core::Time reset_pulse_interval { 0 };
        core::Time presence_pulse_interval { 0 };
        core::Time write_slot_interval { 0 };
        core::Time write_bit_interval { 0 };
        core::Time write_recovery_interval { 0 };
        core::Time read_slot_interval { 0 };
        core::Time read_bit_init_interval { 0 };
        core::Time read_bit_rc_interval { 0 };
        core::Time read_recovery_interval { 0 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p delayer to perform required delays between bus operations.
    //!  - @p gpio to read/write data to the bus.
    Bus(system::IDelayer& delayer, io::IGpio& gpio, Params params);

    //! Reset the bus.
    status::StatusCode reset();

    //! Write @p bit to the bus.
    status::StatusCode write_bit(uint8_t bit);

    //! Read @p bit from the bus.
    status::StatusCode read_bit(uint8_t& bit);

    //! Write @p byte to the bus.
    status::StatusCode write_byte(uint8_t byte);

    //! Read @p byte from the bus.
    status::StatusCode read_byte(uint8_t& byte);

    //! Read @p size bytes from the bus to @p buf.
    //!
    //! @remarks
    //!  - @p buf should be at least @p size bytes long.
    status::StatusCode read_bytes(uint8_t* buf, unsigned size);

    //! Write @p size bytes from @p buf to the bus.
    //!
    //! @remarks
    //!  - @p buf should be at least @p size bytes long.
    status::StatusCode write_bytes(const uint8_t* buf, unsigned size);

private:
    status::StatusCode write_bit_one_();
    status::StatusCode write_bit_zero_();

    const Params params_;

    const unsigned bits_in_byte_ = 8;

    core::Time reset_remain_time_slot_ { 0 };
    core::Time write_remain_time_slot_ { 0 };
    core::Time read_remain_time_slot_ { 0 };

    system::IDelayer& delayer_;
    io::IGpio& gpio_;
};

} // namespace onewire
} // namespace ocs
