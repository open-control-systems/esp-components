/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "ocs_io/gpio/types.h"
#include "ocs_onewire/bus.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_sensor/ds18b20/sensor.h"
#include "ocs_status/code.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

class Store : public scheduler::ITask, public core::NonCopyable<> {
public:
    using SensorList = std::vector<Sensor*>;

    using Func =
        std::function<status::StatusCode(onewire::Bus& bus, SensorList& sensors)>;

    //! Initialize.
    //!
    //! @params
    //!  - @p max_event_count - maximum number of asynchronous events that can be
    //!    scheduled per 1-Wire bus. If the value is too small and the run() is called
    //!    rarely, it's possible to miss some events.
    explicit Store(unsigned max_event_count);

    //! Handle asynchronous events on the 1-wire buses.
    status::StatusCode run() override;

    //! Add sensor to the bus.
    //!
    //! @notes
    //!  - If no bus exists, it will be created automatically.
    //!  - Single bus can contain several sensors.
    //!
    //! @remarks
    //!  - @p sensor should be used in the same context as a run() method, in other words,
    //!    the sensor and the store should be scheduled on the same task scheduler.
    status::StatusCode add(Sensor& sensor, io::gpio::Gpio gpio, const char* gpio_id);

    //! Schedule an asynchronous event to the bus.
    scheduler::AsyncFuncScheduler::FuturePtr schedule(io::gpio::Gpio gpio, Func func);

private:
    class Node : public scheduler::ITask, public core::NonCopyable<> {
    public:
        //! Initialize.
        Node(io::gpio::Gpio gpio, const char* gpio_id, unsigned max_event_count);

        //! Handle operations on the 1-Wire bus.
        status::StatusCode run() override;

        //! Associate @p sensor with the bus.
        status::StatusCode add(Sensor& sensor);

        //! Schedule an asynchronous event to the bus.
        scheduler::AsyncFuncScheduler::FuturePtr schedule(Func func);

    private:
        scheduler::AsyncFuncScheduler func_scheduler_;

        std::unique_ptr<io::gpio::IGpio> gpio_;
        std::unique_ptr<system::IDelayer> delayer_;
        std::unique_ptr<onewire::Bus> bus_;
        SensorList sensors_;
    };

    using NodePtr = std::shared_ptr<Node>;
    using NodeListItem = std::pair<io::gpio::Gpio, NodePtr>;
    using NodeList = std::vector<NodeListItem>;

    const unsigned max_event_count_ { 0 };

    NodePtr get_node_(io::gpio::Gpio gpio);
    NodePtr add_node_(io::gpio::Gpio gpio, const char* gpio_id);

    NodeList nodes_;
};

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
