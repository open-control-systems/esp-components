/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_security/sha_to_hex_str.h"

namespace ocs {
namespace system {

class DeviceID : public core::NonCopyable<> {
public:
    //! Initialize.
    DeviceID();

    //! Return human-readable unique string device identifier.
    const char* get_id() const;

private:
    std::unique_ptr<security::sha_to_hex_str> id_;
};

} // namespace system
} // namespace ocs
