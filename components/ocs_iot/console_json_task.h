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
#include "ocs_iot/default_json_formatter.h"
#include "ocs_iot/ijson_formatter.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace iot {

//! Component to print sensor data to the console.
template <unsigned JsonSize>
class ConsoleJsonTask : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p log_tag to distinguish one console task from another.
    //!  - @p formatter to format an actual data.
    ConsoleJsonTask(const char* log_tag, IJsonFormatter& formatter)
        : log_tag_(log_tag)
        , formatter_(formatter) {
        json_formatter_.reset(new (std::nothrow) JsonFormatter());
        configASSERT(json_formatter_);
    }

    status::StatusCode run() override {
        auto json = CjsonUniqueBuilder::make_json();

        formatter_.format(json.get());
        json_formatter_->format(json.get());

        ESP_LOGI(log_tag_.c_str(), "%s", json_formatter_->c_str());

        return status::StatusCode::OK;
    }

private:
    using JsonFormatter = DefaultJsonFormatter<JsonSize>;

    const std::string log_tag_;

    IJsonFormatter& formatter_;

    std::unique_ptr<JsonFormatter> json_formatter_;
};

} // namespace iot
} // namespace ocs
