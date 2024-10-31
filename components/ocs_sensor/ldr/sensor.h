/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_core/spmc_node.h"
#include "ocs_io/iadc.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace sensor {
namespace ldr {

class Sensor : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Various sensor characteristics.
    struct Data {
        int raw { 0 };
        int voltage { 0 };
        int lightness { 0 };
    };

    struct Params {
        unsigned value_min { 0 };
        unsigned value_max { 0 };
    };

    //! Initialize.
    Sensor(io::IAdc& adc, Params params);

    //! Read sensor data.
    status::StatusCode run() override;

    //! Return the latest sensor data.
    Data get_data() const;

private:
    int calculate_lightness_(int raw) const;

    void update_data_(int raw, int voltage);

    const Params params_;

    io::IAdc& adc_;
    core::SpmcNode<Data> data_;
};

} // namespace ldr
} // namespace sensor
} // namespace ocs
