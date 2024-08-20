/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_iot/fanout_json_formatter.h"
#include "ocs_iot/http_command_handler.h"
#include "ocs_iot/http_data_handler.h"
#include "ocs_iot/http_server_pipeline.h"
#include "ocs_scheduler/itask.h"

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
#include "ocs_iot/http_system_state_handler.h"
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY

namespace ocs {
namespace iot {

class HttpPipeline : public core::NonCopyable<> {
public:
    struct DataParams {
        //! Buffer size to hold the formatted JSON data, in bytes.
        unsigned buffer_size { 0 };
    };

    struct Params {
        DataParams telemetry;
        DataParams registration;
        DataParams commands;
    };

    //! Initialize.
    HttpPipeline(scheduler::ITask& reboot_task,
                 scheduler::ITask& control_task,
                 IJsonFormatter& telemetry_formatter,
                 FanoutJsonFormatter& registration_formatter,
                 Params params);

    //! Start the pipeline.
    status::StatusCode start();

private:
    status::StatusCode register_mdns_endpoints_();

    std::unique_ptr<HttpServerPipeline> http_server_pipeline_;
    std::unique_ptr<HttpDataHandler> http_telemetry_handler_;
    std::unique_ptr<HttpDataHandler> http_registration_handler_;
    std::unique_ptr<HttpCommandHandler> http_command_handler_;
    std::unique_ptr<IJsonFormatter> network_formatter_;

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    std::unique_ptr<HttpSystemStateHandler> http_system_state_handler_;
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY
};

} // namespace iot
} // namespace ocs
