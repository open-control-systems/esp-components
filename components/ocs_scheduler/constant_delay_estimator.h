/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/idelay_estimator.h"

namespace ocs {
namespace scheduler {

class ConstantDelayEstimator : public IDelayEstimator, public core::NonCopyable<> {
public:
    //! Initialize.
    explicit ConstantDelayEstimator(TickType_t delay);

    //! Begin delay estimation.
    void begin() override;

    //! Return the configured delay.
    TickType_t estimate() override;

private:
    const TickType_t delay_ { pdMS_TO_TICKS(0) };
};

} // namespace scheduler
} // namespace ocs
