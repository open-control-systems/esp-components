/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

//! Static FreeRTOS event group.
class StaticEventGroup : public NonCopyable<> {
public:
    //! Initialize.
    StaticEventGroup();

    //! Return event group handler.
    EventGroupHandle_t get() const;

private:
    StaticEventGroup_t buf_;
    EventGroupHandle_t group_;
};

} // namespace core
} // namespace ocs
