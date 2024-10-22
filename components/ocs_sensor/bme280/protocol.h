/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>

#include "ocs_core/macros.h"

namespace ocs {
namespace sensor {
namespace bme280 {

//! Register address.
using RegisterAddress = uint8_t;

//! The “id” register contains the chip identification number, which is 0x60. This
//! number can be read as soon as the device finished the power-on-reset.
struct OCS_ATTR_PACKED RegisterID {
    static const RegisterAddress address = 0xD0;

    uint8_t value { 0 };
};
static_assert(sizeof(RegisterID) == 1);

//! If the value 0xB6 is written to the register, the device is reset using the
//! complete power-on-reset procedure. Writing other values than 0xB6 has no effect.
//! The readout value is always 0x00.
struct OCS_ATTR_PACKED RegisterReset {
    static const RegisterAddress address = 0xE0;
    static const uint8_t reset_value = 0xB6;

    uint8_t value { 0 };
};
static_assert(sizeof(RegisterReset) == 1);

//! The “ctrl_hum” register sets the humidity data acquisition options of the
//! device. Changes to this register only become effective after a write operation
//! to “ctrl_meas”.
struct OCS_ATTR_PACKED RegisterCtrlHum {
    static const RegisterAddress address = 0xF2;

    //! Controls oversampling of humidity data.
    uint8_t osrs_h : 3 { 0 };
    uint8_t reseved : 5 { 0 };
};
static_assert(sizeof(RegisterCtrlHum) == 1);

//! The “status” register contains two bits which indicate the status of the device.
struct OCS_ATTR_PACKED RegisterStatus {
    static const RegisterAddress address = 0xF3;

    //! Automatically set to ‘1’ when the NVM data are being copied to image registers
    //! and back to ‘0’ when the copying is done. The data are copied at
    //! power-on-reset and before every conversion.
    uint8_t im_update : 1 { 0 };

    //! Bits 1,2 are reserved.
    uint8_t reserved0 : 2 { 0 };

    //! Automatically set to ‘1’ whenever a conversion is running and back to ‘0’ when
    //! the results have been transferred to the data registers.
    uint8_t measuring : 1 { 0 };

    //! Bits 4, 5, 6, 7 are reserved.
    uint8_t reserved1 : 4 { 0 };
};
static_assert(sizeof(RegisterStatus) == 1);

//! Register sets the pressure and temperature data acquisition options of the device.
struct OCS_ATTR_PACKED RegisterCtrlMeas {
    static const RegisterAddress address = 0xF4;

    //! Controls the sensor mode of the device.
    uint8_t mode : 2 { 0 };

    //! Controls oversampling of pressure data.
    uint8_t osrs_p : 3 { 0 };

    //! Controls oversampling of temperature data.
    uint8_t osrs_t : 3 { 0 };
};
static_assert(sizeof(RegisterCtrlMeas) == 1);

//! The “config” register sets the rate, filter and interface options of the device.
//! Writes to the “config” register in normal mode may be ignored. In sleep mode
//! writes are not ignored.
struct OCS_ATTR_PACKED RegisterConfig {
    static const RegisterAddress address = 0xF5;

    //! Enables 3-wire SPI interface when set to ‘1’.
    uint8_t spi3w_en : 1 { 0 };

    //! Reserved bit.
    uint8_t reserved : 1 { 0 };

    //! Controls the time constant of the IIR (Infinite Impulse Response) filter.
    //!
    //! |  Value  | Coefficient |
    //! | --------| ------------|
    //! |   000   | Filter off  |
    //! |   001   |   2         |
    //! |   010   |   4         |
    //! |   011   |   8         |
    //! |   100   |   16        |
    uint8_t filter : 3 { 0 };

    //! Control inactive duration t_standby in normal mode.
    //!
    //! | Value   | Duration (ms) |
    //! |---------|---------------|
    //! |   0     |   000.5       |
    //! |   001   |   62.5        |
    //! |   010   |   125         |
    //! |   011   |   250         |
    //! |   100   |   500         |
    //! |   101   |   1000        |
    //! |   110   |   10          |
    //! |   111   |   20          |
    uint8_t t_sb : 3 { 0 };
};
static_assert(sizeof(RegisterConfig) == 1);

struct OCS_ATTR_PACKED RegisterData {
    //! 0xF7 - 0xFE.
    static const RegisterAddress address = 0xF7;

    //! Contains the MSB part [19:12] of the raw pressure measurement output data.
    uint8_t press_msb { 0 };

    //! Contains the LSB part [11:4] of the raw pressure measurement output data.
    uint8_t press_lsb { 0 };

    //! Contains the XLSB part [3:0] of the raw pressure measurement output data.
    //! Contents depend on temperature resolution.
    uint8_t press_xlsb { 0 };

    //! Contains the MSB part [19:12] of the raw temperature measurement output data.
    uint8_t temp_msb { 0 };

    //! Contains the LSB part [11:4] of the raw temperature measurement output data.
    uint8_t temp_lsb { 0 };

    //! Contains the XLSB part [3:0] of the raw temperature measurement output data.
    uint8_t temp_xlsb { 0 };

    //! Contains the MSB part [15:8] of the raw humidity measurement output data.
    uint8_t hum_msb { 0 };

    //! Contains the LSB part [7:0] of the raw humidity measurement output data.
    uint8_t hum_lsb { 0 };
};
static_assert(sizeof(RegisterData) == 8);

struct OCS_ATTR_PACKED CalibrationData1 {
    // 0x88-0xA1.
    static const RegisterAddress address = 0x88;

    uint16_t dig_T1 { 0 };
    int16_t dig_T2 { 0 };
    int16_t dig_T3 { 0 };
    uint16_t dig_P1 { 0 };
    int16_t dig_P2 { 0 };
    int16_t dig_P3 { 0 };
    int16_t dig_P4 { 0 };
    int16_t dig_P5 { 0 };
    int16_t dig_P6 { 0 };
    int16_t dig_P7 { 0 };
    int16_t dig_P8 { 0 };
    int16_t dig_P9 { 0 };
    uint8_t reserved0 { 0 };
    uint8_t dig_H1 { 0 };
};
static_assert(sizeof(CalibrationData1) == 26);

struct CalibrationData2 {
    // 0xE1-0xE7.
    int16_t dig_H2 { 0 };
    uint8_t dig_H3 { 0 };
    int16_t dig_H4 { 0 };
    int16_t dig_H5 { 0 };
    int8_t dig_H6 { 0 };
};

} // namespace bme280
} // namespace sensor
} // namespace ocs
