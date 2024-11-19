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
#include "ocs_pipeline/jsonfmt/version_formatter.h"
#include "ocs_system/device_info.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class RegistrationFormatter : public fmt::json::IFormatter, public core::NonCopyable<> {
public:
    //! Initialize.
    explicit RegistrationFormatter(const system::DeviceInfo& device_info);

    //! Format the underlying data into @p json.
    status::StatusCode format(cJSON* json) override;

    fmt::json::FanoutFormatter& get_fanout_formatter();

private:
    std::unique_ptr<fmt::json::FanoutFormatter> fanout_formatter_;
    std::unique_ptr<VersionFormatter> version_formatter_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
