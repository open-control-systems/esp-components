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

#include "esp_log.h"

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/noncopyable.h"
#include "ocs_iot/cjson_builder.h"
#include "ocs_iot/dynamic_json_formatter.h"
#include "ocs_iot/ijson_formatter.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace iot {

//! Component to print sensor data to the console.
class ConsoleJsonTask : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p formatter to format an actual data.
    //!  - @p log_tag to distinguish one console task from another.
    //!  - @p buffer_size to hold the formatted JSON data, in bytes.
    ConsoleJsonTask(IJsonFormatter& formatter, const char* log_tag, unsigned buffer_size);

    //! Write data to the console.
    status::StatusCode run() override;

private:
    const std::string log_tag_;

    IJsonFormatter& formatter_;

    std::unique_ptr<DynamicJsonFormatter> json_formatter_;
};

} // namespace iot
} // namespace ocs
