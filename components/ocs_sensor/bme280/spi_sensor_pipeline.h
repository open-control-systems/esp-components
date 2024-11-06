/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_core/time.h"
#include "ocs_io/gpio/types.h"
#include "ocs_io/spi/istore.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/bme280/itransceiver.h"
#include "ocs_sensor/bme280/sensor.h"

namespace ocs {
namespace sensor {
namespace bme280 {

class SpiSensorPipeline : public core::NonCopyable<> {
public:
    struct Params {
        core::Time read_interval { 0 };
        io::gpio::Gpio cs_gpio { static_cast<io::gpio::Gpio>(-1) };
        Sensor::Params sensor;
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p task_scheduler to register a task for periodic sensor reading.
    //!  - @p store to register a new SPI device.
    SpiSensorPipeline(scheduler::ITaskScheduler& task_scheduler,
                      io::spi::IStore& store,
                      Params params);

    //! Return the underlying sensor.
    Sensor& get_sensor();

private:
    io::spi::IStore::ITransceiverPtr spi_transceiver_;
    std::unique_ptr<ITransceiver> register_transceiver_;
    std::unique_ptr<Sensor> sensor_;
};

} // namespace bme280
} // namespace sensor
} // namespace ocs
