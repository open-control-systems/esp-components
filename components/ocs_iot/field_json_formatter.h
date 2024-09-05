/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_iot/cjson_builder.h"
#include "ocs_iot/ijson_formatter.h"

namespace ocs {
namespace iot {

class FieldJsonFormatter : public IJsonFormatter, public core::NonCopyable<> {
public:
    enum class Type : uint8_t {
        Object,
        Array,
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p key - JSON key.
    //!  - @p type - JSON object type.
    FieldJsonFormatter(const char* key, Type type = Type::Object);

    //! Format data into @p json.
    status::StatusCode format(cJSON* json) override;

    //! Add additional JSON formatter.
    void add(IJsonFormatter& formatter);

private:
    CjsonUniqueBuilder::Ptr make_field_() const;

    const std::string key_;
    const Type type_ { Type::Object };

    std::vector<IJsonFormatter*> formatters_;
};

} // namespace iot
} // namespace ocs
