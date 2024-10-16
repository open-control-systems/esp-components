/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/macros.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace sensor {
namespace bme280 {

class SpiSensorPipeline : public core::NonCopyable<> {
public:
    struct Params {
        Protocol::Params protocol;
        core::microseconds_t read_interval { 0 };
    };

    //! Initialize.
    SpiSensorPipeline(spi::IStore& store, Params params);

private:
};

} // namespace bme280
} // namespace sensor
} // namespace ocs
