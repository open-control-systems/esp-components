/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_onewire/rom_code.h"
#include "ocs_sensor/ds18b20/resolution_to_str.h"
#include "ocs_sensor/ds18b20/scratchpad.h"
#include "ocs_sensor/ds18b20/sensor.h"
#include "ocs_status/code_to_str.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

namespace {

void configuration_resolution_to_proto(uint8_t& r0,
                                       uint8_t& r1,
                                       Sensor::Configuration::Resolution resolution) {
    switch (resolution) {
    case Sensor::Configuration::Resolution::Bit_12: {
        r0 = 1;
        r1 = 1;
        break;
    }

    case Sensor::Configuration::Resolution::Bit_11: {
        r0 = 0;
        r1 = 1;
        break;
    }

    case Sensor::Configuration::Resolution::Bit_10: {
        r0 = 1;
        r1 = 0;
        break;
    }

    case Sensor::Configuration::Resolution::Bit_9: {
        r0 = 0;
        r1 = 0;
        break;
    }

    // Should not happen.
    default:
        break;
    }
}

const char* log_tag = "ds18b20-sensor";

} // namespace

Sensor::Sensor(storage::IStorage& storage, const char* id)
    : BasicSensor(id)
    , storage_(storage) {
}

status::StatusCode Sensor::run() {
    OCS_STATUS_RETURN_ON_FALSE(configured(), status::StatusCode::InvalidState);

    // Start temperature conversation.
    OCS_STATUS_RETURN_ON_ERROR(configuration_.rom_code.match(*bus_));

    OCS_STATUS_RETURN_ON_ERROR(
        bus_->write_byte(static_cast<uint8_t>(Scratchpad::Commands::ConvertT)));

    // Read temperature from scratchpad.
    Scratchpad scratchpad;
    OCS_STATUS_RETURN_ON_ERROR(scratchpad.read(*bus_, configuration_.rom_code));

    if (scratchpad.valid()) {
        set_data_(scratchpad.get_temperature());
    }

    return status::StatusCode::OK;
}

bool Sensor::configured() const {
    return bus_;
}

status::StatusCode Sensor::read_configuration(Sensor::Configuration& configuration) {
    if (configured()) {
        configuration = configuration_;
        return status::StatusCode::OK;
    }

    return storage_.read(id(), &configuration, sizeof(configuration));
}

status::StatusCode
Sensor::write_configuration(onewire::Bus& bus,
                            const Sensor::Configuration& configuration) {
    if (!configuration.rom_code.valid()) {
        ocs_logw(
            log_tag,
            "can't update configuration: attempting to write invalid rom code: id=%s "
            "serial_number=" DS_SENSOR_SERIAL_NUMBER_STR "",
            id(), DS_SENSOR_SERIAL_NUMBER_TO_STR(configuration.rom_code.serial_number));

        return status::StatusCode::InvalidArg;
    }

    // Write resolution to sensor. As all the data is already in persisted storage, the
    // sensor's EEPROM isn't used. So there's no need to copy the scratchpad from sensor's
    // memory to EEPROM.
    uint8_t r0 = 0;
    uint8_t r1 = 0;
    configuration_resolution_to_proto(r0, r1, configuration.resolution);

    Scratchpad scratchpad;
    scratchpad.configuration.r0 = r0;
    scratchpad.configuration.r1 = r1;
    OCS_STATUS_RETURN_ON_ERROR(scratchpad.write(bus, configuration.rom_code));

    // Ensure that the sensor configuration has been updated.
    OCS_STATUS_RETURN_ON_ERROR(scratchpad.read(bus, configuration.rom_code));
    if (!scratchpad.valid()) {
        ocs_loge(log_tag,
                 "can't update configuration: sensor scratchpad is invalid: id=%s", id());

        return status::StatusCode::InvalidState;
    }

    if (scratchpad.configuration.r0 != r0 || scratchpad.configuration.r1 != r1) {
        ocs_loge(log_tag,
                 "can't update configuration: sensor scratchpad isn't updated: id=%s: "
                 "want: r0=%u r1=%u; got: "
                 "r0=%u r1=%u",
                 id(), r0, r1, scratchpad.configuration.r0, scratchpad.configuration.r1);

        return status::StatusCode::InvalidState;
    }

    // Read current configuration to reduce unnecessary writes to the flash.
    Sensor::Configuration persisted_configuration;

    const auto code =
        storage_.read(id(), &persisted_configuration, sizeof(persisted_configuration));
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        ocs_loge(
            log_tag,
            "can't update configuration: failed to read configuration: id=%s code=%s",
            id(), status::code_to_str(code));
        return code;
    }

    if (persisted_configuration != configuration) {
        ocs_logi(log_tag,
                 "changing configuration: cur: resolution=%s "
                 "serial_number=" DS_SENSOR_SERIAL_NUMBER_STR "; new: "
                 "resolution=%s serial_number= " DS_SENSOR_SERIAL_NUMBER_STR "",
                 resolution_to_str(persisted_configuration.resolution),
                 DS_SENSOR_SERIAL_NUMBER_TO_STR(
                     persisted_configuration.rom_code.serial_number),
                 resolution_to_str(configuration.resolution),
                 DS_SENSOR_SERIAL_NUMBER_TO_STR(configuration.rom_code.serial_number));

        // Write configuration to the persistent storage.
        OCS_STATUS_RETURN_ON_ERROR(
            storage_.write(id(), &configuration, sizeof(configuration)));
    }

    // Configure sensor.
    bus_ = &bus;
    configuration_ = configuration;

    ocs_logi(log_tag,
             "sensor configured: id=%s serial_number=" DS_SENSOR_SERIAL_NUMBER_STR
             " resolution=%s",
             id(), DS_SENSOR_SERIAL_NUMBER_TO_STR(configuration_.rom_code.serial_number),
             resolution_to_str(configuration_.resolution));

    return status::StatusCode::OK;
}

status::StatusCode Sensor::erase_configuration(Sensor::Configuration& configuration) {
    OCS_STATUS_RETURN_ON_ERROR(storage_.erase(id()));

    configuration = configuration_;

    bus_ = nullptr;
    configuration_ = Sensor::Configuration();

    return status::StatusCode::OK;
}

bool Sensor::Configuration::operator==(const Sensor::Configuration& configuration) const {
    return memcmp(this, &configuration, sizeof(Configuration)) == 0;
}

bool Sensor::Configuration::operator!=(const Sensor::Configuration& configuration) const {
    return !(*this == configuration);
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
