/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <functional>

#include "ocs_core/noncopyable.h"
#include "ocs_http/server.h"
#include "ocs_net/imdns_driver.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_sensor/sht41/sensor.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class SHT41Handler : public core::NonCopyable<> {
public:
    //! Initialize.
    SHT41Handler(scheduler::AsyncFuncScheduler& func_scheduler,
                 http::Server& http_server,
                 net::IMdnsDriver& mdns_driver,
                 sensor::sht41::Sensor& sensor);

private:
    using HandleOperationFunc = std::function<status::StatusCode(sensor::sht41::Sensor&)>;

    status::StatusCode handle_operation_(httpd_req_t* req, HandleOperationFunc func);

    static const TickType_t wait_op_interval_ { pdMS_TO_TICKS(5 * 1000) };

    scheduler::AsyncFuncScheduler& func_scheduler_;
    sensor::sht41::Sensor& sensor_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
