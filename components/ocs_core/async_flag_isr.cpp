/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_core/async_flag_isr.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace core {

status::StatusCode AsyncFlagIsr::signal() {
    TaskHandle_t task_to_notify = acquire_task_();
    OCS_STATUS_RETURN_ON_FALSE(task_to_notify, status::StatusCode::Error);

    BaseType_t high_task_woken = pdFALSE;
    vTaskNotifyGiveFromISR(task_to_notify, &high_task_woken);
    OCS_STATUS_RETURN_ON_FALSE(high_task_woken == pdTRUE, status::StatusCode::OK);

    return status::StatusCode::IsrYield;
}

} // namespace core
} // namespace ocs
