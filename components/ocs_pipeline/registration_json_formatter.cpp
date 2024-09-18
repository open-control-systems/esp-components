/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_core/version.h"
#include "ocs_core/version_to_str.h"
#include "ocs_pipeline/registration_json_formatter.h"

namespace ocs {
namespace pipeline {

namespace {

const char* log_tag = "registration-json-formatter";

} // namespace

RegistrationJsonFormatter::RegistrationJsonFormatter() {
    fanout_formatter_.reset(new (std::nothrow) fmt::json::FanoutFormatter());
    configASSERT(fanout_formatter_);

    core::Version version;
    if (!version.parse(CONFIG_OCS_CORE_FW_VERSION)) {
        ocs_loge(log_tag, "failed to parse FW version: %s", CONFIG_OCS_CORE_FW_VERSION);
    }

    core::version_to_str version_str(version);

    version_formatter_.reset(new (std::nothrow) VersionJsonFormatter());
    configASSERT(version_formatter_);

    version_formatter_->add("version_scs", version_str.c_str());
    fanout_formatter_->add(*version_formatter_);
}

status::StatusCode RegistrationJsonFormatter::format(cJSON* json) {
    return fanout_formatter_->format(json);
}

fmt::json::FanoutFormatter& RegistrationJsonFormatter::get_fanout_formatter() {
    return *fanout_formatter_;
}

} // namespace pipeline
} // namespace ocs
