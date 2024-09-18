/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <string>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_fmt/json/iformatter.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace pipeline {

//! Component to print sensor data to the console.
class ConsoleJsonTask : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p formatter to format an actual data.
    //!  - @p log_tag to distinguish one console task from another.
    //!  - @p buffer_size to hold the formatted JSON data, in bytes.
    ConsoleJsonTask(fmt::json::IFormatter& formatter,
                    const char* log_tag,
                    unsigned buffer_size);

    //! Write data to the console.
    status::StatusCode run() override;

private:
    const std::string log_tag_;

    fmt::json::IFormatter& formatter_;

    std::unique_ptr<fmt::json::DynamicFormatter> json_formatter_;
};

} // namespace pipeline
} // namespace ocs
