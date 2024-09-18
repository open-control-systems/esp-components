/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_pipeline/system_state_json_formatter.h"
#include "ocs_system/system_state_builder.h"
#include "ocs_system/task_state_to_str.h"

namespace ocs {
namespace pipeline {

namespace {

status::StatusCode format_task_state(fmt::json::CjsonObjectFormatter& formatter,
                                     configRUN_TIME_COUNTER_TYPE total_time,
                                     const TaskStatus_t& state) {
    if (!formatter.add_string_cs("name", state.pcTaskName)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_string_ref_cs("state",
                                     system::task_state_to_str(state.eCurrentState))) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("number", state.xTaskNumber)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("priority", state.uxCurrentPriority)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("stack_free", state.usStackHighWaterMark)) {
        return status::StatusCode::NoMem;
    }

#ifdef CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
    if (!formatter.add_number_cs("core_id", state.xCoreID)) {
        return status::StatusCode::NoMem;
    }
#endif // CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID

#ifdef CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS
    if (total_time) {
        if (!formatter.add_number_cs("runtime_abs", state.ulRunTimeCounter)) {
            return status::StatusCode::NoMem;
        }

        const unsigned decimal_places = 2;
        const auto value = static_cast<double>(state.ulRunTimeCounter) / total_time;
        const auto multiplier = std::pow(10.0, decimal_places);
        const auto relative = std::ceil(value * multiplier) / multiplier;

        if (!formatter.add_number_cs("runtime_rel", relative)) {
            return status::StatusCode::NoMem;
        }
    }
#endif // CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS

    return status::StatusCode::OK;
}

} // namespace

status::StatusCode SystemStateJsonFormatter::format(cJSON* json) {
    auto array = cJSON_AddArrayToObject(json, "tasks");
    if (!array) {
        return status::StatusCode::NoMem;
    }

    system::SystemStateBuilder state_builder;
    const auto& system_state = state_builder.get();

    fmt::json::CjsonUniqueBuilder builder;

    for (const auto& task_state : system_state.states) {
        auto json = builder.make_object();
        if (!json) {
            return status::StatusCode::NoMem;
        }

        fmt::json::CjsonObjectFormatter formatter(json.get());
        const auto code =
            format_task_state(formatter, system_state.total_time / 100UL, task_state);
        if (code != status::StatusCode::OK) {
            return code;
        }

        if (!cJSON_AddItemToArray(array, json.get())) {
            return status::StatusCode::NoMem;
        }

        json.release();
    }

    return status::StatusCode::OK;
}

} // namespace pipeline
} // namespace ocs
