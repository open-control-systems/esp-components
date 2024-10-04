/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_fmt/json/func_formatter.h"

namespace ocs {
namespace fmt {
namespace json {

FuncFormatter::FuncFormatter(FuncFormatter::Func func)
    : func_(func) {
}

status::StatusCode FuncFormatter::format(cJSON* json) {
    return func_(json);
}

} // namespace json
} // namespace fmt
} // namespace ocs
