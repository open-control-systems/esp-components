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
#include "ocs_pipeline/jsonfmt/registration_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

namespace {

const char* log_tag = "registration_json_formatter";

} // namespace

RegistrationFormatter::RegistrationFormatter(const system::DeviceInfo& device_info) {
    fanout_formatter_.reset(new (std::nothrow) fmt::json::FanoutFormatter());
    configASSERT(fanout_formatter_);

    version_formatter_.reset(new (std::nothrow) VersionFormatter());
    configASSERT(version_formatter_);

    core::Version version;
    if (version.parse(device_info.get_fw_version())) {
        version_formatter_->add("fw_version", device_info.get_fw_version());
    } else {
        ocs_loge(log_tag, "failed to parse FW version: %s", device_info.get_fw_version());
        version_formatter_->add("fw_version", "<none>");
    }

    version_formatter_->add("fw_name", device_info.get_fw_name());
    version_formatter_->add("fw_description", device_info.get_fw_description());
    version_formatter_->add("device_id", device_info.get_device_id());

    fanout_formatter_->add(*version_formatter_);
}

status::StatusCode RegistrationFormatter::format(cJSON* json) {
    return fanout_formatter_->format(json);
}

fmt::json::FanoutFormatter& RegistrationFormatter::get_fanout_formatter() {
    return *fanout_formatter_;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
