/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/bme280/spi_sensor_pipeline.h"
#include "ocs_sensor/bme280/spi_transceiver.h"

namespace ocs {
namespace sensor {
namespace bme280 {

SpiSensorPipeline::SpiSensorPipeline(scheduler::ITaskScheduler& task_scheduler,
                                     io::spi::IStore& store,
                                     SpiSensorPipeline::Params params) {
    configASSERT(params.read_interval);

    spi_transceiver_ = store.add("bme280", params.cs_gpio, 0, 10 * 1000 * 1000);
    configASSERT(spi_transceiver_);

    register_transceiver_.reset(new (std::nothrow) SpiTransceiver(*spi_transceiver_));
    configASSERT(register_transceiver_);

    sensor_.reset(new (std::nothrow) Sensor(*register_transceiver_, params.sensor));
    configASSERT(sensor_);

    configASSERT(task_scheduler.add(*sensor_, "bme280_task", params.read_interval)
                 == status::StatusCode::OK);
}

Sensor& SpiSensorPipeline::get_sensor() {
    return *sensor_;
}

} // namespace bme280
} // namespace sensor
} // namespace ocs
