/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cmath>

#include "ocs_iot/cjson_builder.h"
#include "ocs_iot/cjson_object_formatter.h"
#include "ocs_iot/system_state_json_formatter.h"
#include "ocs_system/system_state_builder.h"
#include "ocs_system/task_state_to_str.h"

namespace ocs {
namespace iot {

namespace {

void format_task_state(CjsonObjectFormatter& formatter,
                       configRUN_TIME_COUNTER_TYPE total_time,
                       const TaskStatus_t& state) {
    formatter.add_string_cs("name", state.pcTaskName);
    formatter.add_string_ref_cs("state", system::task_state_to_str(state.eCurrentState));
    formatter.add_number_cs("number", state.xTaskNumber);
    formatter.add_number_cs("priority", state.uxCurrentPriority);
    formatter.add_number_cs("stack_free", state.usStackHighWaterMark);

#ifdef CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
    formatter.add_number_cs("core_id", state.xCoreID);
#endif // CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID

#ifdef CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS
    if (total_time) {
        formatter.add_number_cs("runtime_abs", state.ulRunTimeCounter);

        const unsigned decimal_places = 2;
        const auto value = static_cast<double>(state.ulRunTimeCounter) / total_time;
        const auto multiplier = std::pow(10.0, decimal_places);
        const auto relative = std::ceil(value * multiplier) / multiplier;

        formatter.add_number_cs("runtime_rel", relative);
    }
#endif // CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS
}

} // namespace

void SystemStateJsonFormatter::format(cJSON* json) {
    auto array = cJSON_AddArrayToObject(json, "tasks");
    configASSERT(array);

    system::SystemStateBuilder state_builder;
    const auto& system_state = state_builder.get();

    CjsonUniqueBuilder builder;

    for (const auto& task_state : system_state.states) {
        auto json = builder.make_json();
        configASSERT(json);

        CjsonObjectFormatter formatter(json.get());
        format_task_state(formatter, system_state.total_time / 100UL, task_state);

        configASSERT(cJSON_AddItemToArray(array, json.get()));
        json.release();
    }
}

} // namespace iot
} // namespace ocs
