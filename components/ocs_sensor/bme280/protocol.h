/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>

namespace ocs {
namespace sensor {
namespace bme {

//! References
//!  https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
struct Protocol : {
    //! Number of times a value (temperature, pressure, humidity) is measured before
    //! available to be read. By default a value is measured once.
    //!
    //! @remarks
    //!  More number of the measurements, more power is consumed.
    enum class OversamplingMode : uint8_t {
        None = 0b00000000,
        One = 0b00000001,
        Two = 0b00000010,
        Four = 0b00000011,
        Eight = 0b00000100,
        Sixteen = 0b00000101,
    };

    //! Various sensor operational modes. See section 3.3 page 14.
    enum class OperationMode : uint8_t {
        //! No operation, all registers accessible, lowest power, selected after startup.
        Sleep = 0b00000000,

        //! Perform one measurement, store results and return to sleep mode.
        Forced = 0b00000010,

        //! Perpetual cycling of measurements and inactive periods.
        Normal = 0b00000011,
    };

    struct Params {
        OperationMode operation_mode { OperationMode::Forced };
        OversamplingMode temperature_oversampling { OversamplingMode::One };
        OversamplingMode humidity_oversampling { OversamplingMode::One };
        OversamplingMode pressure_oversampling { OversamplingMode::One };
    };

    //! Value: 0xD0.
    //!
    //! Description:
    //!  The “id” register contains the chip identification number chip_id[7:0], which is
    //!  0x60. This number can be read as soon as the device finished the power-on-reset.
    using RegisterID = uint8_t;

    //! Value: 0xE0.
    //!
    //! Description:
    //!  The “reset” register contains the soft reset word reset[7:0]. If the value 0xB6
    //!  is written to the register, the device is reset using the complete power-on-reset
    //!  procedure. Writing other values than 0xB6 has no effect. The readout value is
    //!  always 0x00.
    using RegisterReset = uint8_t;

    //! Value: 0xF2.
    //!
    //! Description:
    //!   The “ctrl_hum” register sets the humidity data acquisition options of the
    //!   device. Changes to this register only become effective after a write operation
    //!   to “ctrl_meas”.
    struct OCS_ATTR_PACKED RegisterCtrlHum : uint8_t {
        //! Controls oversampling of humidity data.
        uint8_t osrs_h : 3 { 0 };
        uint8_t reseved : 5 { 0 };
    };
    static_assert(sizeof(RegisterCtrlMeas) == 1);

    //! Value: 0xF3.
    //!
    //! Description:
    //!  The “status” register contains two bits which indicate the status of the device.
    struct OCS_ATTR_PACKED RegisterStatus : uint8_t {
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
    struct OCS_ATTR_PACKED RegisterCtrlMeas : uint8_t {
        //! Controls the sensor mode of the device.
        uint8_t mode : 2 { 0 };

        //! Controls oversampling of pressure data.
        uint8_t osrs_p : 3 { 0 };

        //! Controls oversampling of temperature data.
        uint8_t osrs_t : 3 { 0 };
    };
    static_assert(sizeof(RegisterCtrlMeas) == 1);

    //! Table 26 page 30.
    struct OCS_ATTR_PACKED Configuration {
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

        //! Controls inactive duration t_standby in normal mode.
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
    static_assert(sizeof(Configuration) == 1);

    //! Value: 0xF7 - 0xFE.
    struct OCS_ATTR_PACKED DataRegister : uint64_t {
        //! Contains the XLSB part [3:0] of the raw pressure measurement output data.
        //! Contents depend on temperature resolution.
        uint8_t press_xlsb : 4 { 0 };

        //! Contains the LSB part [11:4] of the raw pressure measurement output data.
        uint8_t press_lsb { 0 };

        //! Contains the MSB part [19:12] of the raw pressure measurement output data.
        uint8_t press_msb { 0 };

        //! Contains the XLSB part [3:0] of the raw temperature measurement output data.
        uint8_t temp_xlsb : 4 { 0 };

        //! Contains the LSB part [11:4] of the raw temperature measurement output data.
        uint8_t temp_lsb { 0 };

        //! Contains the MSB part [19:12] of the raw temperature measurement output data.
        uint8_t temp_msb { 0 };

        //! Contains the LSB part [15:8] of the raw humidity measurement output data.
        uint8_t temp_lsb { 0 };

        //! Contains the MSB part [7:0] of the raw humidity measurement output data.
        uint8_t temp_msb { 0 };

        //! Reserved bits.
        uint8_t reserved { 0 };
    };
    static_assert(sizeof(DataRegister) == 8);
};

} // namespace bme
} // namespace sensor
} // namespace ocs
