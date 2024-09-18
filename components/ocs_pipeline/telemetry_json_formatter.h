/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace pipeline {

class TelemetryJsonFormatter : public fmt::json::IFormatter, public core::NonCopyable<> {
public:
    //! Initialize.
    TelemetryJsonFormatter();

    //! Format all telemetry data into @p json.
    status::StatusCode format(cJSON* json);

    fmt::json::FanoutFormatter& get_fanout_formatter();

private:
    std::unique_ptr<fmt::json::FanoutFormatter> fanout_formatter_;
    std::unique_ptr<fmt::json::IFormatter> system_formatter_;
};

} // namespace pipeline
} // namespace ocs
