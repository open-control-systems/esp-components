/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/operation_guard.h"

namespace ocs {
namespace core {

OperationGuard::OperationGuard() {
    vTaskSuspendAll();
}

OperationGuard::~OperationGuard() {
    if (xTaskResumeAll() == pdFALSE) {
        taskYIELD();
    }
}

} // namespace core
} // namespace ocs
