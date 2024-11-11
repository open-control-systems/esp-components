/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/bit_ops.h"
#include "ocs_algo/crc_ops.h"
#include "ocs_algo/math_ops.h"
#include "ocs_core/log.h"
#include "ocs_sensor/sht41/sensor.h"
#include "ocs_status/code_to_str.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace sensor {
namespace sht41 {

namespace {

uint8_t calculate_crc(uint8_t hi, uint8_t lo) {
    const uint8_t buf[2] { hi, lo };
    return algo::CrcOps::crc8(buf, sizeof(buf), 0xFF, 0x31, algo::CrcOps::BitOrder::MSB);
}

const char* log_tag = "sht41_sensor";

} // namespace

Sensor::Sensor(io::i2c::ITransceiver& transceiver,
               storage::IStorage& storage,
               Sensor::Params params)
    : params_(params)
    , transceiver_(transceiver)
    , storage_(storage) {
    configASSERT(params_.send_wait_interval);
    configASSERT(params_.bus_wait_interval);

    if (const auto code = read_heating_count_(); code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to read sensor heating count from storage: code=%s",
                 status::code_to_str(code));
    }

    reset();

    if (const auto code = read_serial_number_(); code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to read sensor serial number: code=%s",
                 status::code_to_str(code));
    }

    heating_delay_ = estimate_heating_delay_(params.heating_command);
    configASSERT(heating_delay_);

    ocs_logi(log_tag,
             "serial_number=0x%8lX measure_command=%s "
             "heating_command=%s heating_delay=%lu(ms) heating_count=%u",
             serial_number_, command_to_str_(params_.measure_command),
             command_to_str_(params.heating_command), pdTICKS_TO_MS(heating_delay_),
             heating_count_);
}

status::StatusCode Sensor::run() {
    OCS_STATUS_RETURN_ON_ERROR(send_command_(params_.measure_command));

    Data data;
    data.heating_count = heating_count_;

    OCS_STATUS_RETURN_ON_ERROR(receive_data_(data));

    data_.set(data);

    return status::StatusCode::OK;
}

Sensor::Data Sensor::get_data() const {
    return data_.get();
}

status::StatusCode Sensor::reset() {
    const auto code = reset_();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to reset sensor: %s", status::code_to_str(code));
    } else {
        ocs_logi(log_tag, "reset completed");
    }

    return code;
}

status::StatusCode Sensor::heat() {
    ocs_logi(log_tag, "start heating");

    const auto code = heat_();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "heating failed: %s", status::code_to_str(code));
    } else {
        ocs_logi(log_tag, "heating completed");
    }

    return code;
}

const char* Sensor::command_to_str_(Sensor::Command command) {
    switch (command) {
    case Command::MeasureHighPrecision:
        return "measure_high_precision";

    case Command::MeasureMediumPrecision:
        return "measure_medium_precision";

    case Command::MeasureLowPrecision:
        return "measure_low_precision";

    case Command::ReadSerialNumber:
        return "read_serial_number";

    case Command::SoftReset:
        return "soft_reset";

    case Command::ActivateHeater_200mW_1000ms:
        return "activate_heater_200mW_1000ms";

    case Command::ActivateHeater_200mW_100ms:
        return "activate_heater_200mW_100ms";

    case Command::ActivateHeater_110mW_1000ms:
        return "activate_heater_110mW_1000ms";

    case Command::ActivateHeater_110mW_100ms:
        return "activate_heater_110mW_100ms";

    case Command::ActivateHeater_20mW_1000ms:
        return "activate_heater_20mW_1000ms";

    case Command::ActivateHeater_20mW_100ms:
        return "activate_heater_20mW_100ms";
    }

    return "<none>";
}

TickType_t Sensor::estimate_heating_delay_(Command command) {
    switch (command) {
    case Command::ActivateHeater_200mW_1000ms:
        return heating_long_pulse_delay_;

    case Command::ActivateHeater_200mW_100ms:
        return heating_short_pulse_delay_;

    case Command::ActivateHeater_110mW_1000ms:
        return heating_long_pulse_delay_;

    case Command::ActivateHeater_110mW_100ms:
        return heating_short_pulse_delay_;

    case Command::ActivateHeater_20mW_1000ms:
        return heating_long_pulse_delay_;

    case Command::ActivateHeater_20mW_100ms:
        return heating_short_pulse_delay_;

    default:
        break;
    }

    return 0;
}

status::StatusCode Sensor::reset_() {
    OCS_STATUS_RETURN_ON_ERROR(send_command_(Command::SoftReset));

    return status::StatusCode::OK;
}

status::StatusCode Sensor::heat_() {
    OCS_STATUS_RETURN_ON_ERROR(send_command_(params_.heating_command));

    vTaskDelay(heating_delay_);

    Data data;
    OCS_STATUS_RETURN_ON_ERROR(receive_data_(data));

    ++heating_count_;
    data.heating_count = heating_count_;

    if (const auto code = write_heating_count_(); code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to persist sensor heating count: %s",
                 status::code_to_str(code));
    }

    data_.set(data);

    return status::StatusCode::OK;
}

status::StatusCode Sensor::read_serial_number_() {
    OCS_STATUS_RETURN_ON_ERROR(send_command_(Command::ReadSerialNumber));

    uint8_t buf[6];
    memset(buf, 0, sizeof(buf));
    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_.receive(buf, sizeof(buf), params_.bus_wait_interval));

    const uint16_t hi = algo::BitOps::pack_u8(buf[0], buf[1]);
    const uint8_t hi_checksum = buf[2];
    const uint8_t hi_checksum_calculated = calculate_crc(buf[0], buf[1]);
    if (hi_checksum != hi_checksum_calculated) {
        ocs_logw(
            log_tag,
            "failed to read serial number: invalid CRC for `hi` word: want=%u got=%u",
            hi_checksum, hi_checksum_calculated);

        return status::StatusCode::InvalidState;
    }

    const uint16_t lo = algo::BitOps::pack_u8(buf[3], buf[4]);
    const uint8_t lo_checksum = buf[5];
    const uint8_t lo_checksum_calculated = calculate_crc(buf[3], buf[4]);
    if (lo_checksum != lo_checksum_calculated) {
        ocs_logw(
            log_tag,
            "failed to read serial number: invalid CRC for `lo` word: want=%u got=%u",
            lo_checksum, lo_checksum_calculated);

        return status::StatusCode::InvalidState;
    }

    serial_number_ = algo::BitOps::pack_u16(hi, lo);

    return status::StatusCode::OK;
}

status::StatusCode Sensor::receive_data_(Sensor::Data& data) {
    uint8_t buf[6];
    memset(buf, 0, sizeof(buf));
    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_.receive(buf, sizeof(buf), params_.bus_wait_interval));

    const uint16_t temperature_ticks = algo::BitOps::pack_u8(buf[0], buf[1]);
    const uint8_t temperature_checksum = buf[2];
    const uint8_t temperature_checksum_calculated = calculate_crc(buf[0], buf[1]);
    if (temperature_checksum == temperature_checksum_calculated) {
        data.temperature = -45 + (175.0 * temperature_ticks / UINT16_MAX);
        data.temperature = algo::MathOps::round_floor(data.temperature, 2);
    } else {
        ocs_logw(log_tag, "failed to read temperature: checksum mismatch: want=%u got=%u",
                 temperature_checksum, temperature_checksum_calculated);
    }

    const uint16_t humidity_ticks = algo::BitOps::pack_u8(buf[3], buf[4]);
    const uint8_t humidity_checksum = buf[5];
    const uint8_t humidity_checksum_calculcated = calculate_crc(buf[3], buf[4]);
    if (humidity_checksum == humidity_checksum_calculcated) {
        data.humidity = -6 + (125.0 * humidity_ticks / UINT16_MAX);
        data.humidity = algo::MathOps::round_floor(data.humidity, 2);
        data.humidity = std::clamp(data.humidity, 0.0, 100.0);
    } else {
        ocs_logw(log_tag, "failed to read humidity: checksum mismatch: want=%u got=%u",
                 humidity_checksum, humidity_checksum_calculcated);
    }

    return status::StatusCode::OK;
}

status::StatusCode Sensor::send_command_(Sensor::Command command) {
    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_.send(reinterpret_cast<const uint8_t*>(&command), sizeof(command),
                          params_.bus_wait_interval));

    vTaskDelay(params_.send_wait_interval);

    return status::StatusCode::OK;
}

status::StatusCode Sensor::read_heating_count_() {
    const auto code =
        storage_.read(heating_count_key_, &heating_count_, sizeof(heating_count_));
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode Sensor::write_heating_count_() {
    const auto code =
        storage_.write(heating_count_key_, &heating_count_, sizeof(heating_count_));
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

} // namespace sht41
} // namespace sensor
} // namespace ocs
