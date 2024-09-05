/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_iot/ijson_formatter.h"

namespace ocs {
namespace iot {

class BasicJsonFormatter : public IJsonFormatter,
                           public core::NonCopyable<BasicJsonFormatter> {
public:
    //! Initialize.
    //!
    //! @params
    //!  -@p flat_formatting - enable/disable flat JSON formatting.
    //!
    //!   Flat JSON formatting enabled:
    //!    { "sensor_ldr_raw": 23, "sensor_ldr_voltage": 25, "sensor_ldr_lightness": 10 }
    //!
    //!   Flat JSON formatting disabled:
    //!    {
    //!     "room_1": {
    //!         "ldr_point_1": {"raw": 123, "voltage": 125", "lightness": 23 },
    //!         "ldr_point_2": {"raw": 123, "voltage": 125", "lightness": 32 },
    //!         "ldr_point_3": {"raw": 123, "voltage": 125", "lightness": 97 }
    //!     }
    //!     "room_2": {
    //!         "ldr_point_1": {"raw": 123, "voltage": 125", "lightness": 3 },
    //!         "ldr_point_2": {"raw": 123, "voltage": 125", "lightness": 2 },
    //!         "ldr_point_3": {"raw": 123, "voltage": 125", "lightness": 7 }
    //!     }
    //!    }
    //!
    //!   By default, all JSON formatters should use flat formatting to reduce the overall
    //!   size of the JSON. There are cases where this isn't possible, for example when
    //!   there are multiple sensors in the same family that are used to measure different
    //!   things.
    explicit BasicJsonFormatter(bool flat_formatting);

    virtual ~BasicJsonFormatter() = default;

protected:
    bool flat_formatting_ { false };
};

} // namespace iot
} // namespace ocs
