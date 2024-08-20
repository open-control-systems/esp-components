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
#include "ocs_iot/fanout_json_formatter.h"
#include "ocs_iot/ijson_formatter.h"

namespace ocs {
namespace iot {

class TelemetryJsonFormatter : public IJsonFormatter, public core::NonCopyable<> {
public:
    //! Initialize.
    TelemetryJsonFormatter();

    //! Format all telemetry data into @p json.
    status::StatusCode format(cJSON* json);

    FanoutJsonFormatter& get_fanout_formatter();

private:
    std::unique_ptr<FanoutJsonFormatter> fanout_formatter_;
    std::unique_ptr<IJsonFormatter> system_formatter_;
};

} // namespace iot
} // namespace ocs
