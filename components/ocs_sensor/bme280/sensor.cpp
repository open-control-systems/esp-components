/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/math_ops.h"
#include "ocs_core/bit_ops.h"
#include "ocs_core/log.h"
#include "ocs_sensor/bme280/protocol.h"
#include "ocs_sensor/bme280/sensor.h"
#include "ocs_status/code_to_str.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace sensor {
namespace bme280 {

namespace {

uint32_t format_20bit_data(uint8_t msb, uint8_t lsb, uint8_t xlsb) {
    const uint16_t high_bits = core::BitOps::pack_u8(msb, lsb);
    const uint8_t low_bits = (xlsb & 0xF0) >> 4;

    return static_cast<uint32_t>(high_bits) << 4 | low_bits;
}

uint8_t oversampling_to_coef(Sensor::OversamplingMode oversampling_mode) {
    switch (oversampling_mode) {
    case Sensor::OversamplingMode::None:
        break;
    case Sensor::OversamplingMode::One:
        return 1;
    case Sensor::OversamplingMode::Two:
        return 2;
    case Sensor::OversamplingMode::Four:
        return 4;
    case Sensor::OversamplingMode::Eight:
        return 8;
    case Sensor::OversamplingMode::Sixteen:
        return 16;
    }

    return 0;
}

} // namespace

Sensor::Sensor(ITransceiver& transceiver, const char* id, Sensor::Params params)
    : BasicSensor(id)
    , params_(params)
    , transceiver_(transceiver) {
    estimate_measurement_time_();

    if (const auto code = reset_(); code != status::StatusCode::OK) {
        ocs_loge(id_.c_str(), "failed to reset sensor: code=%s",
                 status::code_to_str(code));
    }

    if (const auto code = read_serial_number_(); code != status::StatusCode::OK) {
        ocs_loge(id_.c_str(), "failed to read sensor serial number: code=%s",
                 status::code_to_str(code));
    }

    if (const auto code = write_configuration_(); code != status::StatusCode::OK) {
        ocs_loge(id_.c_str(), "failed to write sensor configuration: code=%s",
                 status::code_to_str(code));
    }

    if (const auto code = read_configuration_(); code != status::StatusCode::OK) {
        ocs_loge(id_.c_str(), "failed to read sensor configuration: code=%s",
                 status::code_to_str(code));
    }

    if (const auto code = read_calibration1_(); code != status::StatusCode::OK) {
        ocs_loge(id_.c_str(), "failed to read sensor calibration1: code=%s",
                 status::code_to_str(code));
    }

    if (const auto code = read_calibration2_(); code != status::StatusCode::OK) {
        ocs_loge(id_.c_str(), "failed to read sensor calibration2: code=%s",
                 status::code_to_str(code));
    }

    //! Allow the sensor to stabilise before the actual measurements.
    if (const auto code = run(); code != status::StatusCode::OK) {
        ocs_loge(id_.c_str(), "failed to read sensor data at startup: code=%s",
                 status::code_to_str(code));
    }
}

status::StatusCode Sensor::run() {
    if (params_.operation_mode == OperationMode::Forced) {
        OCS_STATUS_RETURN_ON_ERROR(write_configuration_());

        while (true) {
            RegisterStatus status_register;

            OCS_STATUS_RETURN_ON_ERROR(
                transceiver_.receive(reinterpret_cast<uint8_t*>(&status_register),
                                     sizeof(status_register), RegisterStatus::address));

            if (!status_register.measuring) {
                break;
            }

            vTaskDelay(wait_measurement_interval_);
        }
    }

    return read_data_();
}

void Sensor::estimate_measurement_time_() {
    // Appendix B: measurement time and current calculation, page 51.
    const unsigned typ_measurement_duration =
        std::ceil(1 + (2 * oversampling_to_coef(params_.temperature_oversampling))
                  + ((2 * oversampling_to_coef(params_.pressure_oversampling)) + 0.5)
                  + ((2 * oversampling_to_coef(params_.humidity_oversampling)) + 0.5));

    const unsigned max_measurement_duration = std::ceil(
        1.25 + (2.3 * oversampling_to_coef(params_.temperature_oversampling))
        + ((2.3 * oversampling_to_coef(params_.pressure_oversampling)) + 0.575)
        + ((2.3 * oversampling_to_coef(params_.humidity_oversampling)) + 0.575));

    ocs_logi(id_.c_str(), "time measurement estimated: cur=%lu(ms) typ=%u(ms) max=%u(ms)",
             pdTICKS_TO_MS(wait_measurement_interval_), typ_measurement_duration,
             max_measurement_duration);
}

status::StatusCode Sensor::reset_() {
    RegisterReset reset_register;
    reset_register.value = RegisterReset::reset_value;

    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_.send(reinterpret_cast<const uint8_t*>(&reset_register),
                          sizeof(reset_register), RegisterReset::address));

    // Give the sensor time to reset.
    vTaskDelay(wait_reset_interval_);

    return status::StatusCode::OK;
}

status::StatusCode Sensor::read_serial_number_() {
    RegisterID id_register;

    OCS_STATUS_RETURN_ON_ERROR(transceiver_.receive(
        &id_register.value, sizeof(id_register), RegisterID::address));

    ocs_logi(id_.c_str(), "serial_number=0x%02X", id_register.value);

    return status::StatusCode::OK;
}

status::StatusCode Sensor::write_configuration_() {
    ocs_logi(
        id_.c_str(),
        "writing configuration: mode=%u osrs_p=%u osrs_t=%u osrs_h=%u filter=%u t_sb=%u",
        static_cast<uint8_t>(params_.operation_mode),
        static_cast<uint8_t>(params_.pressure_oversampling),
        static_cast<uint8_t>(params_.temperature_oversampling),
        static_cast<uint8_t>(params_.humidity_oversampling),
        static_cast<uint8_t>(params_.iir_coefficient),
        static_cast<uint8_t>(params_.inactive_duration));

    RegisterCtrlHum humidity_register;
    humidity_register.osrs_h = static_cast<uint8_t>(params_.humidity_oversampling);

    RegisterCtrlMeas measure_register;
    measure_register.mode = static_cast<uint8_t>(params_.operation_mode);
    measure_register.osrs_p = static_cast<uint8_t>(params_.pressure_oversampling);
    measure_register.osrs_t = static_cast<uint8_t>(params_.temperature_oversampling);

    RegisterConfig config_register;
    config_register.filter = static_cast<uint8_t>(params_.iir_coefficient);
    config_register.t_sb = params_.inactive_duration;

    uint8_t send_buf[4];
    memset(send_buf, 0, sizeof(send_buf));

    memcpy(send_buf, &humidity_register, 1);
    memcpy(send_buf + 2, &measure_register, 1);
    memcpy(send_buf + 3, &config_register, 1);

    return transceiver_.send(send_buf, sizeof(send_buf), RegisterCtrlHum::address);
}

status::StatusCode Sensor::read_configuration_() {
    RegisterCtrlHum humidity_register;
    RegisterCtrlMeas measure_register;
    RegisterConfig config_register;

    uint8_t recv_buf[4];
    memset(recv_buf, 0, sizeof(recv_buf));

    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_.receive(recv_buf, sizeof(recv_buf), RegisterCtrlHum::address));

    memcpy(&humidity_register, recv_buf, 1);
    memcpy(&measure_register, recv_buf + 2, 1);
    memcpy(&config_register, recv_buf + 3, 1);

    ocs_logi(id_.c_str(),
             "sensor configuration: ctrl_hum:[osrs_h=%u]; ctrl_meas=[mode=%u osrs_p=%u "
             "osrs_t=%u]; config=[spi3w_en=%u filter=%u t_sb=%u]",
             humidity_register.osrs_h, measure_register.mode, measure_register.osrs_p,
             measure_register.osrs_t, config_register.spi3w_en, config_register.filter,
             config_register.t_sb);

    return status::StatusCode::OK;
}

status::StatusCode Sensor::read_calibration1_() {
    uint8_t recv_buf[sizeof(CalibrationData1)];
    memset(recv_buf, 0, sizeof(recv_buf));

    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_.receive(recv_buf, sizeof(recv_buf), CalibrationData1::address));

    memcpy(&calibration1_, recv_buf, sizeof(calibration1_));

    ocs_logi(id_.c_str(),
             "sensor calibration 1: T1=%u T2=%i T3=%i P1=%u P2=%i P3=%i P4=%i P5=%i "
             "P6=%i P7=%i P8=%i P9=%i H1=%u",
             calibration1_.dig_T1, calibration1_.dig_T2, calibration1_.dig_T3,
             calibration1_.dig_P1, calibration1_.dig_P2, calibration1_.dig_P3,
             calibration1_.dig_P4, calibration1_.dig_P5, calibration1_.dig_P6,
             calibration1_.dig_P7, calibration1_.dig_P8, calibration1_.dig_P9,
             calibration1_.dig_H1);

    return status::StatusCode::OK;
}

status::StatusCode Sensor::read_calibration2_() {
    // 7 registers.
    const unsigned calibration_register_count = 0xE7 - 0xE1 + 1;

    uint8_t recv_buf[calibration_register_count];
    memset(recv_buf, 0, sizeof(recv_buf));

    OCS_STATUS_RETURN_ON_ERROR(transceiver_.receive(recv_buf, sizeof(recv_buf), 0xE1));

    calibration2_.dig_H2 =
        static_cast<int16_t>(core::BitOps::pack_u8(recv_buf[1], recv_buf[0]));

    calibration2_.dig_H3 = recv_buf[2];

    const uint8_t dig_h4_high_bits = recv_buf[3];
    const uint8_t dig_h4_low_bits = recv_buf[4] & 0x0F;

    calibration2_.dig_H4 =
        static_cast<int16_t>((dig_h4_high_bits) << 4 | dig_h4_low_bits);

    const uint8_t dig_h5_high_bits = recv_buf[5];
    const uint8_t dig_h5_low_bits = (recv_buf[4] & 0xF0) >> 4;

    calibration2_.dig_H5 =
        static_cast<int16_t>((dig_h5_high_bits << 4) | dig_h5_low_bits);

    calibration2_.dig_H6 = static_cast<int8_t>(recv_buf[6]);

    ocs_logi(id_.c_str(), "sensor calibration 2: H2=%i H3=%u H4=%i H5=%i H6=%i",
             calibration2_.dig_H2, calibration2_.dig_H3, calibration2_.dig_H4,
             calibration2_.dig_H5, calibration2_.dig_H6);

    return status::StatusCode::OK;
}

status::StatusCode Sensor::read_data_() {
    RegisterData register_data;

    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_.receive(reinterpret_cast<uint8_t*>(&register_data),
                             sizeof(register_data), RegisterData::address));

    const int32_t pressure_raw = static_cast<int32_t>(format_20bit_data(
        register_data.press_msb, register_data.press_lsb, register_data.press_xlsb));

    const int32_t temperature_raw = static_cast<int32_t>(format_20bit_data(
        register_data.temp_msb, register_data.temp_lsb, register_data.temp_xlsb));

    const int32_t humidity_raw = static_cast<int32_t>(
        core::BitOps::pack_u8(register_data.hum_msb, register_data.hum_lsb));

    SensorData sensor_data;

    sensor_data.pressure =
        static_cast<double>(BME280_compensate_P_int64_(pressure_raw)) / 256.0;

    if (params_.pressure_resolution) {
        sensor_data.pressure /= std::pow(10.0, params_.pressure_resolution);
    }
    if (params_.pressure_decimal_places) {
        sensor_data.pressure = algo::MathOps::round_floor(
            sensor_data.pressure, params_.pressure_decimal_places);
    }

    sensor_data.temperature =
        static_cast<double>(BME280_compensate_T_int32_(temperature_raw)) / 100.0;

    sensor_data.humidity =
        static_cast<double>(BME280_compensate_H_int32_(humidity_raw)) / 1024.0;

    if (params_.humidity_decimal_places) {
        sensor_data.humidity = algo::MathOps::round_floor(
            sensor_data.humidity, params_.humidity_decimal_places);
    }

    set_data_(sensor_data);

    return status::StatusCode::OK;
}

Sensor::BME280_S32_t Sensor::BME280_compensate_T_int32_(Sensor::BME280_S32_t adc_T) {
    BME280_S32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((Sensor::BME280_S32_t)calibration1_.dig_T1 << 1)))
            * ((Sensor::BME280_S32_t)calibration1_.dig_T2))
        >> 11;
    var2 = (((((adc_T >> 4) - ((Sensor::BME280_S32_t)calibration1_.dig_T1))
              * ((adc_T >> 4) - ((Sensor::BME280_S32_t)calibration1_.dig_T1)))
             >> 12)
            * ((Sensor::BME280_S32_t)calibration1_.dig_T3))
        >> 14;
    t_fine_ = var1 + var2;
    T = (t_fine_ * 5 + 128) >> 8;
    return T;
}

Sensor::BME280_U32_t Sensor::BME280_compensate_P_int64_(Sensor::BME280_S32_t adc_P) {
    Sensor::BME280_S64_t var1, var2, p;
    var1 = ((Sensor::BME280_S64_t)t_fine_) - 128000;
    var2 = var1 * var1 * (Sensor::BME280_S64_t)calibration1_.dig_P6;
    var2 = var2 + ((var1 * (Sensor::BME280_S64_t)calibration1_.dig_P5) << 17);
    var2 = var2 + (((Sensor::BME280_S64_t)calibration1_.dig_P4) << 35);
    var1 = ((var1 * var1 * (Sensor::BME280_S64_t)calibration1_.dig_P3) >> 8)
        + ((var1 * (Sensor::BME280_S64_t)calibration1_.dig_P2) << 12);
    var1 = (((((Sensor::BME280_S64_t)1) << 47) + var1))
            * ((Sensor::BME280_S64_t)calibration1_.dig_P1)
        >> 33;
    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((Sensor::BME280_S64_t)calibration1_.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((Sensor::BME280_S64_t)calibration1_.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((Sensor::BME280_S64_t)calibration1_.dig_P7) << 4);
    return (Sensor::BME280_U32_t)p;
}

Sensor::BME280_U32_t Sensor::BME280_compensate_H_int32_(BME280_S32_t adc_H) {
    Sensor::BME280_S32_t v_x1_u32r;
    v_x1_u32r = (t_fine_ - ((Sensor::BME280_S32_t)76800));
    v_x1_u32r =
        (((((adc_H << 14) - (((Sensor::BME280_S32_t)calibration2_.dig_H4) << 20)
            - (((Sensor::BME280_S32_t)calibration2_.dig_H5) * v_x1_u32r))
           + ((Sensor::BME280_S32_t)16384))
          >> 15)
         * (((((((v_x1_u32r * ((Sensor::BME280_S32_t)calibration2_.dig_H6)) >> 10)
                * (((v_x1_u32r * ((Sensor::BME280_S32_t)calibration2_.dig_H3)) >> 11)
                   + ((Sensor::BME280_S32_t)32768)))
               >> 10)
              + ((Sensor::BME280_S32_t)2097152))
                 * ((Sensor::BME280_S32_t)calibration2_.dig_H2)
             + 8192)
            >> 14));
    v_x1_u32r = (v_x1_u32r
                 - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7)
                     * ((Sensor::BME280_S32_t)calibration1_.dig_H1))
                    >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    return (Sensor::BME280_U32_t)(v_x1_u32r >> 12);
}

} // namespace bme280
} // namespace sensor
} // namespace ocs
