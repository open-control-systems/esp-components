/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"
#include "ocs_sensor/basic_sensor.h"
#include "ocs_sensor/bme280/itransceiver.h"

namespace ocs {
namespace sensor {
namespace bme280 {

struct SensorData {
    double pressure { 0.0 };
    double temperature { 0.0 };
    double humidity { 0.0 };
};

//! BME280 sensor.
//!
//! @references
//!  https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
class Sensor : public scheduler::ITask,
               public BasicSensor<SensorData>,
               public core::NonCopyable<> {
public:
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

    //! IIR (Infinite Impulse Response) coefficients. Section 3.4.4 page 18.
    enum class IirCoefficient : uint8_t {
        None = 0b00000000,
        Two = 0b00000001,
        Four = 0b00000010,
        Eight = 0b00000011,
        Sixteen = 0b00000100,
    };

    struct Params {
        OperationMode operation_mode { OperationMode::Sleep };
        OversamplingMode temperature_oversampling { OversamplingMode::One };
        OversamplingMode humidity_oversampling { OversamplingMode::One };
        OversamplingMode pressure_oversampling { OversamplingMode::One };
        IirCoefficient iir_coefficient { IirCoefficient::None };

        //! Control inactive duration in normal mode.
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
        uint8_t inactive_duration { 0 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p transceiver to communicate with BME280 sensor.
    //!  - @p id to distinguish one sensor from another.
    Sensor(ITransceiver& transceiver, const char* id, Params params);

    //! Read sensor data.
    status::StatusCode run() override;

private:
    void read_serial_number_();
    void write_configuration_(Params params);
    void read_calibration1_();
    void read_calibration2_();

    using BME280_S32_t = int32_t;
    using BME280_U32_t = uint32_t;
    using BME280_S64_t = int64_t;

    // Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123”
    // equals 51.23 DegC.
    BME280_S32_t BME280_compensate_T_int32_(BME280_S32_t adc_T);

    // Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits
    // and 8 fractional bits). Output value of “24674867” represents
    // 24674867/256 = 96386.2 Pa = 963.862 hPa
    BME280_U32_t BME280_compensate_P_int64_(BME280_S32_t adc_P);

    // Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and
    // 10 fractional bits). Output value of “47445” represents 47445/1024 = 46.333 %RH
    BME280_U32_t BME280_compensate_H_int32_(BME280_S32_t adc_H);

    ITransceiver& transceiver_;

    CalibrationData1 calibration1_;
    CalibrationData2 calibration2_;

    // t_fine carries fine temperature.
    BME280_S32_t t_fine_ { 0 };
};

} // namespace bme280
} // namespace sensor
} // namespace ocs
