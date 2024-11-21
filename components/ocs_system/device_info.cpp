/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_system/device_info.h"

namespace ocs {
namespace system {

DeviceInfo::DeviceInfo(const char* fw_name,
                       const char* fw_version,
                       const char* fw_description)
    : fw_name_(fw_name)
    , fw_version_(fw_version)
    , fw_description_(fw_description) {
}

const char* DeviceInfo::get_fw_name() const {
    return fw_name_.c_str();
}

const char* DeviceInfo::get_fw_description() const {
    return fw_description_.c_str();
}

const char* DeviceInfo::get_fw_version() const {
    return fw_version_.c_str();
}

const char* DeviceInfo::get_device_id() const {
    return device_id_.get_id();
}

} // namespace system
} // namespace ocs
