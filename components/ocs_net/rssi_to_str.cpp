/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/rssi_to_str.h"

namespace ocs {
namespace net {

const char* rssi_to_str(int rssi) {
    if (rssi > -50) {
        return "excellent";
    }

    if (rssi > -60) {
        return "good";
    }

    if (rssi > -70) {
        return "fair";
    }

    if (rssi > -80) {
        return "poor";
    }

    return "unreliable";
}

} // namespace net
} // namespace ocs
