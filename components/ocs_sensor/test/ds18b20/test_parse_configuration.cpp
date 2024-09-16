/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_onewire/serial_number_to_str.h"
#include "ocs_sensor/ds18b20/parse_configuration.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

TEST_CASE("Parse configuration: invalid resolution invalid serial number",
          "[ocs_sensor], [parse_configuration]") {
    Sensor::Configuration configuration;

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      parse_configuration(configuration, "abcd", "0"));
}

TEST_CASE("Parse configuration: valid resolution invalid serial number",
          "[ocs_sensor], [parse_configuration]") {
    Sensor::Configuration configuration;

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      parse_configuration(configuration, "abcd", "9"));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      parse_configuration(configuration, "abcd", "10"));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      parse_configuration(configuration, "abcd", "11"));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      parse_configuration(configuration, "abcd", "12"));
}

TEST_CASE("Parse configuration: invalid resolution valid serial number",
          "[ocs_sensor], [parse_configuration]") {
    Sensor::Configuration configuration;

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      parse_configuration(configuration, "AA:BB:CC:DD:EE:FF", "0"));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      parse_configuration(configuration, "AA:BB:CC:DD:EE:FF", "8"));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      parse_configuration(configuration, "AA:BB:CC:DD:EE:FF", "13"));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      parse_configuration(configuration, "AA:BB:CC:DD:EE:FF", "100"));
}

TEST_CASE("Parse configuration: valid resolution valid serial number",
          "[ocs_sensor], [parse_configuration]") {
    { // 9-Bit resolution
        Sensor::Configuration configuration;

        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          parse_configuration(configuration, "AA:BB:CC:DD:EE:FF", "9"));

        TEST_ASSERT_EQUAL_STRING(
            onewire::serial_number_to_str(configuration.rom_code.serial_number).c_str(),
            "AA:BB:CC:DD:EE:FF");

        TEST_ASSERT_EQUAL(Sensor::Configuration::Resolution::Bit_9,
                          configuration.resolution);
    }

    { // 10-Bit resolution
        Sensor::Configuration configuration;

        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          parse_configuration(configuration, "AA:BB:CC:DD:EE:FF", "10"));

        TEST_ASSERT_EQUAL_STRING(
            onewire::serial_number_to_str(configuration.rom_code.serial_number).c_str(),
            "AA:BB:CC:DD:EE:FF");

        TEST_ASSERT_EQUAL(Sensor::Configuration::Resolution::Bit_10,
                          configuration.resolution);
    }

    { // 11-Bit resolution
        Sensor::Configuration configuration;

        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          parse_configuration(configuration, "AA:BB:CC:DD:EE:FF", "11"));

        TEST_ASSERT_EQUAL_STRING(
            onewire::serial_number_to_str(configuration.rom_code.serial_number).c_str(),
            "AA:BB:CC:DD:EE:FF");

        TEST_ASSERT_EQUAL(Sensor::Configuration::Resolution::Bit_11,
                          configuration.resolution);
    }

    { // 12-Bit resolution
        Sensor::Configuration configuration;

        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          parse_configuration(configuration, "AA:BB:CC:DD:EE:FF", "12"));

        TEST_ASSERT_EQUAL_STRING(
            onewire::serial_number_to_str(configuration.rom_code.serial_number).c_str(),
            "AA:BB:CC:DD:EE:FF");

        TEST_ASSERT_EQUAL(Sensor::Configuration::Resolution::Bit_12,
                          configuration.resolution);
    }
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
