/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/sht41/sensor_pipeline.h"

namespace ocs {
namespace sensor {
namespace sht41 {

SensorPipeline::SensorPipeline(io::i2c::IStore& store,
                               scheduler::ITaskScheduler& task_scheduler,
                               SensorPipeline::Params params) {
    transceiver_ = store.add("sht41", io::i2c::IStore::AddressLength::Bit_7, 0x44,
                             io::i2c::IStore::TransferSpeed::Fast);
    configASSERT(transceiver_);

    sensor_.reset(new (std::nothrow)
                      Sensor(*transceiver_,
                             Sensor::Params {
                                 .send_wait_interval = pdMS_TO_TICKS(10),
                                 .bus_wait_interval = core::Duration::second * 5,
                                 .measure_mode = Sensor::MeasureMode::HighPrecision,
                             }));
    configASSERT(sensor_);

    configASSERT(task_scheduler.add(*sensor_, "sht41_task", params.read_interval)
                 == status::StatusCode::OK);
}

Sensor& SensorPipeline::get_sensor() {
    return *sensor_;
}

} // namespace sht41
} // namespace sensor
} // namespace ocs
