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

class SystemJSONFormatter : public IJSONFormatter, public core::NonCopyable<> {
public:
    //! Format system metrics into @p json.
    void format(cJSON* json) override;
};

} // namespace iot
} // namespace ocs
