/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_fmt/json/time_formatter.h"
#include "ocs_pipeline/httpserver/time_pipeline.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

TimePipeline::TimePipeline(http::Server& server,
                           fmt::json::FanoutFormatter& telemetry_formatter,
                           fmt::json::FanoutFormatter& registration_formatter,
                           time_t start_point) {
    formatter_.reset(new (std::nothrow) fmt::json::TimeFormatter(start_point));
    configASSERT(formatter_);

    telemetry_formatter.add(*formatter_);
    registration_formatter.add(*formatter_);

    handler_.reset(new (std::nothrow) TimeHandler(server, start_point));
    configASSERT(handler_);
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
