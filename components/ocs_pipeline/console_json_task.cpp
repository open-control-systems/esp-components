/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/console_json_task.h"
#include "ocs_core/log.h"

namespace ocs {
namespace pipeline {

ConsoleJsonTask::ConsoleJsonTask(fmt::json::IFormatter& formatter,
                                 const char* log_tag,
                                 unsigned buffer_size)
    : log_tag_(log_tag)
    , formatter_(formatter) {
    json_formatter_.reset(new (std::nothrow) fmt::json::DynamicFormatter(buffer_size));
    configASSERT(json_formatter_);
}

status::StatusCode ConsoleJsonTask::run() {
    auto json = fmt::json::CjsonUniqueBuilder::make_object();
    if (!json) {
        return status::StatusCode::NoMem;
    }

    auto code = formatter_.format(json.get());
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = json_formatter_->format(json.get());
    if (code != status::StatusCode::OK) {
        return code;
    }

    ocs_logi(log_tag_.c_str(), "%s", json_formatter_->c_str());

    return status::StatusCode::OK;
}

} // namespace pipeline
} // namespace ocs
