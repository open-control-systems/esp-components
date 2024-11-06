/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_io/gpio/types.h"
#include "ocs_sensor/ds18b20/sensor.h"
#include "ocs_sensor/ds18b20/store.h"
#include "ocs_test/test_storage.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

namespace {

using TestStorage = test::TestStorage<Sensor::Configuration>;

} // namespace

TEST_CASE("DS18B20 store: schedule: empty store", "[ocs_sensor], [ds18b20_store]") {
    const io::gpio::Gpio gpio = GPIO_NUM_26;

    Store store(16);

    auto future = store.schedule(gpio, [](onewire::Bus& bus, Store::SensorList& sensors) {
        return status::StatusCode::OK;
    });
    TEST_ASSERT_NULL(future);
}

TEST_CASE("DS18B20 store: schedule: invalid GPIO", "[ocs_sensor], [ds18b20_store]") {
    const char* sensor_id = "test_sensor";
    const char* gpio_id = "test_gpio_id";
    const io::gpio::Gpio gpio = GPIO_NUM_26;
    const io::gpio::Gpio invalid_gpio = GPIO_NUM_27;

    TEST_ASSERT_NOT_EQUAL(gpio, invalid_gpio);

    Store store(16);
    TestStorage storage;
    Sensor sensor(storage, sensor_id);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.add(sensor, gpio, gpio_id));

    auto future =
        store.schedule(invalid_gpio, [](onewire::Bus& bus, Store::SensorList& sensors) {
            return status::StatusCode::OK;
        });
    TEST_ASSERT_NULL(future);
}

TEST_CASE("DS18B20 store: add sensor", "[ocs_sensor], [ds18b20_store]") {
    const char* sensor_id = "test_sensor";
    const io::gpio::Gpio gpio = GPIO_NUM_26;
    const char* gpio_id = "test_gpio_id";

    Store store(16);
    TestStorage storage;
    Sensor sensor(storage, sensor_id);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.add(sensor, gpio, gpio_id));

    auto future =
        store.schedule(gpio, [&sensor](onewire::Bus& bus, Store::SensorList& sensors) {
            TEST_ASSERT_EQUAL(1, sensors.size());
            TEST_ASSERT_EQUAL_STRING(sensor.id(), sensors[0]->id());
            return status::StatusCode::OK;
        });
    TEST_ASSERT_NOT_NULL(future);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future->wait(pdMS_TO_TICKS(1000 * 5)));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future->code());
}

TEST_CASE("DS18B20 store: read sensor configuration: non-configured",
          "[ocs_sensor], [ds18b20_store]") {
    const char* sensor_id = "test_sensor";
    const io::gpio::Gpio gpio = GPIO_NUM_26;
    const char* gpio_id = "test_gpio_id";

    Store store(16);
    TestStorage storage;
    Sensor sensor(storage, sensor_id);

    TEST_ASSERT_FALSE(sensor.configured());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.add(sensor, gpio, gpio_id));
    TEST_ASSERT_FALSE(sensor.configured());

    auto future = store.schedule(gpio, [](onewire::Bus& bus, Store::SensorList& sensors) {
        TEST_ASSERT_EQUAL(1, sensors.size());

        TEST_ASSERT_FALSE(sensors[0]->configured());

        Sensor::Configuration configuration;
        TEST_ASSERT_EQUAL(status::StatusCode::NoData,
                          sensors[0]->read_configuration(configuration));

        return status::StatusCode::OK;
    });
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
