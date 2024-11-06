/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/igpio.h"
#include "ocs_io/gpio/types.h"

namespace ocs {
namespace io {
namespace gpio {

class BasicGpio : public IGpio, public core::NonCopyable<BasicGpio> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p id to distinguish one GPIO from another.
    //!  - @p gpio - actual GPIO to operate with.
    //!  - @p enable_value - which value should be used to activate the GPIO.
    BasicGpio(const char* id, Gpio gpio, bool enable_value);

    //! Destroy.
    virtual ~BasicGpio() = default;

    //! Get the GPIO level.
    int get() override;

    //! Change GPIO state to opposite.
    status::StatusCode flip() override;

    //! Enable GPIO.
    status::StatusCode turn_on() override;

    //! Disable GPIO.
    status::StatusCode turn_off() override;

    //! Set the GPIO direction.
    status::StatusCode set_direction(IGpio::Direction direction) override;

private:
    const std::string id_;
    const Gpio gpio_ { GPIO_NUM_NC };
    const bool enable_value_ { false };
};

} // namespace gpio
} // namespace io
} // namespace ocs
