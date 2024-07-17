/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_iot/ijson_formatter.h"

namespace ocs {
namespace iot {

class FanoutJSONFormatter : public IJSONFormatter, public core::NonCopyable<> {
public:
    //! Propogate the call to the underlying formatters.
    void format(cJSON* json) override;

    //! Add @p formatter to be notified when format is called.
    void add(IJSONFormatter& formatter);

private:
    std::vector<IJSONFormatter*> formatters_;
};

} // namespace iot
} // namespace ocs
