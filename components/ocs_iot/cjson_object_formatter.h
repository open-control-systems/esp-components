/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>

#include "cJSON.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace iot {

class cJSONObjectFormatter : public core::NonCopyable<> {
public:
    //! Initialize.
    explicit cJSONObjectFormatter(cJSON* json);

    //! Add constant string @p value with constant @p key to @p json.
    bool add_string_ref_cs(const char* key, const char* value);

    //! Add @p value with constant @p key to @p json.
    bool add_string_cs(const char* key, const char* value);

    //! Add @p value with constant @p key to @p json.
    bool add_number_cs(const char* key, double value);

    //! Add boolean @p value with constant @p key to @p json.
    bool add_bool_cs(const char* key, bool value);

    //! Add True with constant @p key to @p json.
    bool add_true_cs(const char* key);

    //! Add False with constant @p key to @p json.
    bool add_false_cs(const char* key);

    //! Add Null with constant @p key to @p json.
    bool add_null_cs(const char* key);

private:
    cJSON* json_ { nullptr };
};

} // namespace iot
} // namespace ocs
