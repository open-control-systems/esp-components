/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_pipeline/system_json_formatter.h"
#include "ocs_pipeline/telemetry_json_formatter.h"

namespace ocs {
namespace pipeline {

TelemetryJsonFormatter::TelemetryJsonFormatter() {
    fanout_formatter_.reset(new (std::nothrow) fmt::json::FanoutFormatter());
    configASSERT(fanout_formatter_);

    system_formatter_.reset(new (std::nothrow) SystemJsonFormatter());
    configASSERT(system_formatter_);

    fanout_formatter_->add(*system_formatter_);
}

status::StatusCode TelemetryJsonFormatter::format(cJSON* json) {
    return fanout_formatter_->format(json);
}

fmt::json::FanoutFormatter& TelemetryJsonFormatter::get_fanout_formatter() {
    return *fanout_formatter_;
}

} // namespace pipeline
} // namespace ocs
