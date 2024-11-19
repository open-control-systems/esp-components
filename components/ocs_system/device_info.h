/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_system/device_id.h"

namespace ocs {
namespace system {

//! Various device characteristics.
class DeviceInfo : public core::NonCopyable<> {
public:
    //! Initialize.
    DeviceInfo(const char* fw_name, const char* fw_version);

    const char* get_fw_name() const;
    const char* get_fw_version() const;
    const char* get_device_id() const;

private:
    std::string fw_name_;
    std::string fw_version_;
    DeviceID device_id_;
};

} // namespace system
} // namespace ocs