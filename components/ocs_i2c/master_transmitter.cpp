/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_i2c/master_transmitter.h"
#include "ocs_core/log.h"

namespace ocs {
namespace i2c {

namespace {

const char* log_tag = "master-transmitter";

} // namespace

MasterTransmitter::MasterTransmitter(i2_master_dev_handle_t handle, const char* id)
    : id_(id)
    , handle_(handle) {
}

status::StatusCode
transmit(const uint8_t* buf, unsigned size, core::microseconds_t timeout) {
    if (timeout > 0 && timeout < core::Millisecond) {
        return status::StatusCode::InvalidArg;
    }

    const auto err = i2c_master_transmit(handle_, buf, size, timeout);
    if (err != ESP_OK) {
        ocs_logi(log_tag, "i2c_master_transmit() failed: id=%s err=%s", id_.c_str(),
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode receive(uint8_t* buf, unsigned size, core::microseconds_t timeout) {
    if (timeout > 0 && timeout < core::Millisecond) {
        return status::StatusCode::InvalidArg;
    }

    const auto err = i2c_master_receive(handle_, buf, size, timeout);
    if (err != ESP_OK) {
        ocs_logi(log_tag, "i2c_master_receive() failed: id=%s err=%s", id_.c_str(),
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace i2c
} // namespace ocs
