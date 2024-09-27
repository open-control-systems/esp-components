/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/yl69/soil_status_to_str.h"

namespace ocs {
namespace sensor {
namespace yl69 {

const char* soil_status_to_str(SoilStatus status) {
    switch (status) {
    case SoilStatus::Saturated:
        return "Saturated";

    case SoilStatus::Wet:
        return "Wet";

    case SoilStatus::Depletion:
        return "Depletion";

    case SoilStatus::Dry:
        return "Dry";

    case SoilStatus::Error:
        return "Error";

    default:
        break;
    }

    return "<none>";
}

} // namespace yl69
} // namespace sensor
} // namespace ocs
