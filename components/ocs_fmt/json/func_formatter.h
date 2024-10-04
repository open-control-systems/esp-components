/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <functional>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace fmt {
namespace json {

class FuncFormatter : public IFormatter, public core::NonCopyable<> {
public:
    //! Lambda used for JSON formatting.
    using Func = std::function<status::StatusCode(cJSON* json)>;

    //! Initialize
    //!
    //! @params
    //!  @p func to call when format() is called.
    explicit FuncFormatter(Func func);

    //! Call the underlying function to format data into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    Func func_;
};

} // namespace json
} // namespace fmt
} // namespace ocs
