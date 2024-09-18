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
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace fmt {
namespace json {

class FanoutFormatter : public IFormatter, public core::NonCopyable<> {
public:
    //! Propogate the call to the underlying formatters.
    status::StatusCode format(cJSON* json) override;

    //! Add @p formatter to be notified when format is called.
    void add(IFormatter& formatter);

private:
    std::vector<IFormatter*> formatters_;
};

} // namespace json
} // namespace fmt
} // namespace ocs
