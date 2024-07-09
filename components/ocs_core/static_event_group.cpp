/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "ocs_core/static_event_group.h"

namespace ocs {
namespace core {

StaticEventGroup::StaticEventGroup() {
    memset(&buf_, 0, sizeof(buf_));

    group_ = xEventGroupCreateStatic(&buf_);
    configASSERT(group_);
}

EventGroupHandle_t StaticEventGroup::get() const {
    return group_;
}

} // namespace core
} // namespace ocs
