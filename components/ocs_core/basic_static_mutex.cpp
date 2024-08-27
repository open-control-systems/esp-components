/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/basic_static_mutex.h"

namespace ocs {
namespace core {

BasicStaticMutex::BasicStaticMutex() {
    memset(&buff_, 0, sizeof(buff_));

    sem_ = xSemaphoreCreateMutexStatic(&buff_);
    configASSERT(sem_);
}

} // namespace core
} // namespace ocs
