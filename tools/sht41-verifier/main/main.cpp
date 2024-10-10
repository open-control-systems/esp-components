/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

using namespace ocs;

extern "C" void app_main(void) {
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
