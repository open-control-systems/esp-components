/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

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

const char* log_tag = "sensor_bme280";

int32_t format_20bit_data(uint8_t xlsb, uint8_t lsb, uint8_t msb) {
    const uint16_t high_bits = core::BitOps::pack_u8(msb, lsb);
    const uint8_t low_bits = xlsb & 0x0F;

    return static_cast<int32_t>(high_bits) << 4 | low_bits;
}

} // namespace

Sensor::Sensor(ITransceiver& transceiver, const char* id, Sensor::Params params)
    : BasicSensor(id)
    , transceiver_(transceiver) {
    read_serial_number_();
    write_configuration_(params);
    read_calibration1_();
    read_calibration2_();
}

status::StatusCode Sensor::run() {
    RegisterData register_data;

    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_.receive(reinterpret_cast<uint8_t*>(&register_data),
                             sizeof(register_data), RegisterData::address));

    const int32_t pressure_raw = format_20bit_data(
        register_data.press_xlsb, register_data.press_lsb, register_data.press_msb);

    const int32_t temperature_raw = format_20bit_data(
        register_data.temp_xlsb, register_data.temp_lsb, register_data.temp_msb);

    const int32_t humidity_raw = static_cast<int32_t>(
        core::BitOps::pack_u8(register_data.hum_msb, register_data.hum_lsb));

    SensorData sensor_data;

    sensor_data.pressure = static_cast<double>(pressure_raw) / 256.0;
    sensor_data.temperature = static_cast<double>(temperature_raw) / 100.0;
    sensor_data.humidity = static_cast<double>(humidity_raw) / 1024.0;

    set_data_(sensor_data);

    return status::StatusCode::OK;
}

void Sensor::read_serial_number_() {
    RegisterID id_register;

    const auto code = transceiver_.receive(&id_register.value, sizeof(id_register),
                                           RegisterID::address);
    if (code == status::StatusCode::OK) {
        ocs_logi(log_tag, "id=%s serial_number=%u", id_.c_str(), id_register.value);
    } else {
        ocs_loge(log_tag, "failed to read sensor serial number: id=%s code=%s",
                 id_.c_str(), status::code_to_str(code));
    }
}

void Sensor::write_configuration_(Sensor::Params params) {
    RegisterCtrlHum humidity_register;
    humidity_register.osrs_h = static_cast<uint8_t>(params.humidity_oversampling);

    RegisterCtrlMeas measure_register;
    measure_register.mode = static_cast<uint8_t>(params.operation_mode);
    measure_register.osrs_p = static_cast<uint8_t>(params.pressure_oversampling);
    measure_register.osrs_t = static_cast<uint8_t>(params.temperature_oversampling);

    RegisterConfig config_register;
    config_register.filter = static_cast<uint8_t>(params.iir_coefficient);
    config_register.t_sb = params.inactive_duration;

    uint8_t send_buf[4];
    memset(send_buf, 0, sizeof(send_buf));

    memcpy(send_buf, &humidity_register, 1);
    memcpy(send_buf + 2, &measure_register, 1);
    memcpy(send_buf + 3, &config_register, 1);

    const auto code =
        transceiver_.send(send_buf, sizeof(send_buf), RegisterCtrlHum::address);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to write sensor configuration: id=%s code=%s",
                 id_.c_str(), status::code_to_str(code));
    }
}

void Sensor::read_calibration1_() {
    uint8_t send_buf[sizeof(CalibrationData1)];
    memset(send_buf, 0, sizeof(send_buf));

    const auto code = transceiver_.receive(send_buf, sizeof(send_buf), 0x88);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to read sensor calibration: id=%s code=%s", id_.c_str(),
                 status::code_to_str(code));

        return;
    }

    memcpy(&calibration1_, send_buf, sizeof(calibration1_));
}

void Sensor::read_calibration2_() {
    // 7 registers.
    const unsigned calibration_register_count = 0xE7 - 0xE1 + 1;

    uint8_t send_buf[calibration_register_count];
    memset(send_buf, 0, sizeof(send_buf));

    const auto code = transceiver_.receive(send_buf, sizeof(send_buf), 0xE1);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to read sensor calibration: id=%s code=%s", id_.c_str(),
                 status::code_to_str(code));

        return;
    }

    calibration2_.dig_H2 =
        static_cast<int16_t>(core::BitOps::pack_u8(send_buf[1], send_buf[0]));

    calibration2_.dig_H3 = send_buf[2];

    const uint8_t dig_h4_high_bits = send_buf[3];
    const uint8_t dig_h4_low_bits = send_buf[4] & 0x0F;

    calibration2_.dig_H4 =
        static_cast<int16_t>((dig_h4_high_bits) << 4 | dig_h4_low_bits);

    const uint8_t dig_h5_high_bits = send_buf[5];
    const uint8_t dig_h5_low_bits = (send_buf[4] & 0xF0) >> 4;

    calibration2_.dig_H5 =
        static_cast<int16_t>((dig_h5_high_bits << 4) | dig_h5_low_bits);

    calibration2_.dig_H6 = static_cast<int8_t>(send_buf[6]);
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
