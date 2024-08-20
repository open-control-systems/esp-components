/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstring>
#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_iot/ijson_formatter.h"

namespace ocs {
namespace iot {

template <unsigned Size>
class StaticJsonFormatter : public IJsonFormatter, public core::NonCopyable<> {
public:
    //! Initialize.
    StaticJsonFormatter() {
        clear_();
    }

    //! Format @p json to the underlying buffer.
    status::StatusCode format(cJSON* json) override {
        clear_();

        if (!cJSON_PrintPreallocated(json, buf_, sizeof(buf_), false)) {
            return status::StatusCode::NoMem;
        }

        return status::StatusCode::OK;
    }

    //! Return the underlying buffer.
    const char* c_str() const {
        return buf_;
    }

private:
    void clear_() {
        memset(buf_, 0, sizeof(buf_));
    }

    char buf_[Size];
};

} // namespace iot
} // namespace ocs
