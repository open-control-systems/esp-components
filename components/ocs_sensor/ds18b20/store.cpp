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
#include "ocs_io/default_gpio.h"
#include "ocs_sensor/ds18b20/resolution_to_str.h"
#include "ocs_sensor/ds18b20/store.h"
#include "ocs_status/code_to_str.h"
#include "ocs_status/macros.h"
#include "ocs_system/delayer_configuration.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

namespace {

const char* log_tag = "ds18b20-store";

} // namespace

Store::Store(unsigned max_event_count)
    : max_event_count_(max_event_count) {
    configASSERT(max_event_count_);
}

status::StatusCode Store::run() {
    for (auto& [gpio, node] : nodes_) {
        const auto code = node->run();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag, "failed to handle events on the bus: gpio=%d code=%s",
                     static_cast<unsigned>(gpio), status::code_to_str(code));
        }
    }

    return status::StatusCode::OK;
}

scheduler::ITask* Store::add(Sensor& sensor, gpio_num_t gpio, const char* gpio_id) {
    NodePtr node = get_node_(gpio);
    if (!node) {
        node = add_node_(gpio, gpio_id);
    }
    configASSERT(node);

    return node->add(sensor);
}

scheduler::AsyncFuncScheduler::FuturePtr Store::schedule(gpio_num_t gpio,
                                                         Store::Func func) {
    NodePtr node = get_node_(gpio);
    if (!node) {
        return nullptr;
    }

    return node->schedule(func);
}

Store::NodePtr Store::get_node_(gpio_num_t gpio) {
    for (const auto& item : nodes_) {
        if (item.first == gpio) {
            return item.second;
        }
    }

    return nullptr;
}

Store::NodePtr Store::add_node_(gpio_num_t gpio, const char* gpio_id) {
    auto node = NodePtr(new (std::nothrow) Node(gpio, gpio_id, max_event_count_));
    if (!node) {
        return nullptr;
    }

    nodes_.push_back(std::make_pair(gpio, node));

    return node;
}

Store::Node::Node(gpio_num_t gpio, const char* gpio_id, unsigned max_event_count)
    : func_scheduler_(max_event_count) {
    gpio_.reset(new (std::nothrow) io::DefaultGpio(gpio_id, gpio));
    configASSERT(gpio_);

    delayer_ = system::make_delayer(system::DelayerStrategy::Default);
    configASSERT(delayer_);

    // For timing selection, see reference:
    // https://www.analog.com/en/resources/technical-articles/1wire-communication-through-software.html
    bus_.reset(new (std::nothrow)
                   onewire::Bus(*delayer_, *gpio_,
                                onewire::Bus::Params {
                                    .reset_pulse_interval = core::Microsecond * 480,
                                    .presence_pulse_interval = core::Microsecond * 60,
                                    .write_slot_interval = core::Microsecond * 60,
                                    .write_bit_interval = core::Microsecond * 10,
                                    .write_recovery_interval = core::Microsecond * 1,
                                    .read_slot_interval = core::Microsecond * 60,
                                    .read_bit_init_interval = core::Microsecond * 5,
                                    .read_bit_rc_interval = core::Microsecond * 5,
                                    .read_recovery_interval = core::Microsecond * 1,
                                }));
    configASSERT(bus_);
}

status::StatusCode Store::Node::run() {
    auto code = task_scheduler_.wait(wait_interval_);
    if (code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to handle async task events: %s",
                 status::code_to_str(code));
    }

    code = func_scheduler_.run();
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        ocs_logw(log_tag, "failed to handle async func events: %s",
                 status::code_to_str(code));
    }

    return status::StatusCode::OK;
}

scheduler::ITask* Store::Node::add(Sensor& sensor) {
    for (auto& s : sensors_) {
        if (strcmp(s->id(), sensor.id()) == 0) {
            return nullptr;
        }
    }

    Sensor::Configuration configuration;
    auto code = sensor.read_configuration(configuration);
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to load sensor configuration: id=%s code=%s",
                     sensor.id(), status::code_to_str(code));
        }
    } else {
        code = sensor.write_configuration(*bus_, configuration);
        if (code != status::StatusCode::OK) {
            ocs_loge(log_tag, "failed to reload sensor configuration: id=%s code=%s",
                     sensor.id(), status::code_to_str(code));
        } else {
            ocs_logi(log_tag,
                     "reload sensor configuration: id=%s "
                     "serial_number=" DS_SENSOR_SERIAL_NUMBER_STR " resolution=%s",
                     sensor.id(),
                     DS_SENSOR_SERIAL_NUMBER_TO_STR(configuration.rom_code.serial_number),
                     resolution_to_str(configuration.resolution));
        }
    }

    sensors_.push_back(&sensor);
    return task_scheduler_.add(sensor);
}

scheduler::AsyncFuncScheduler::FuturePtr Store::Node::schedule(Func func) {
    return func_scheduler_.add([this, func]() {
        return func(*bus_, sensors_);
    });
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
