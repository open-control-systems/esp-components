/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <unordered_map>

#include "ocs_core/noncopyable.h"
#include "ocs_core/version.h"
#include "ocs_iot/ijson_formatter.h"

namespace ocs {
namespace iot {

class StringJSONFormatter : public IJSONFormatter,
                            public core::NonCopyable<StringJSONFormatter> {
public:
    //! Destroy.
    virtual ~StringJSONFormatter() = default;

    //! Format key-value pairs into @p json.
    void format(cJSON* json) override;

    //! Add key-value pair to be added to the result json when format() is called.
    void add(const char* key, const char* value);

private:
    std::unordered_map<std::string, std::string> values_;
};

} // namespace iot
} // namespace ocs
