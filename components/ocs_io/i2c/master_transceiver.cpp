/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_io/i2c/master_transceiver.h"
#include "ocs_core/log.h"

namespace ocs {
namespace io {
namespace i2c {

namespace {

const char* log_tag = "i2c_master_transceiver";

} // namespace

MasterTransceiver::DevicePtr
MasterTransceiver::make_device_shared(i2c_master_dev_t* device) {
    return MasterTransceiver::DevicePtr(device, [](i2c_master_dev_t* device) {
        if (device) {
            const auto err = i2c_master_bus_rm_device(device);
            if (err != ESP_OK) {
                ocs_loge(log_tag, "i2c_master_bus_rm_device() %s", esp_err_to_name(err));
            }
        }
    });
}

MasterTransceiver::MasterTransceiver(MasterTransceiver::DevicePtr device, const char* id)
    : id_(id)
    , device_(device) {
}

status::StatusCode
MasterTransceiver::send(const uint8_t* buf, unsigned size, core::Time timeout) {
    if (timeout > 0 && timeout < core::Duration::millisecond) {
        return status::StatusCode::InvalidArg;
    }

    const auto err = i2c_master_transmit(device_.get(), buf, size,
                                         timeout / core::Duration::millisecond);
    if (err != ESP_OK) {
        ocs_logi(log_tag, "i2c_master_transmit() failed: id=%s err=%s", id_.c_str(),
                 esp_err_to_name(err));

        if (err == ESP_ERR_INVALID_ARG) {
            return status::StatusCode::InvalidArg;
        }
        if (err == ESP_ERR_TIMEOUT) {
            return status::StatusCode::Timeout;
        }

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode
MasterTransceiver::receive(uint8_t* buf, unsigned size, core::Time timeout) {
    if (timeout > 0 && timeout < core::Duration::millisecond) {
        return status::StatusCode::InvalidArg;
    }

    const auto err = i2c_master_receive(device_.get(), buf, size,
                                        timeout / core::Duration::millisecond);
    if (err != ESP_OK) {
        ocs_logi(log_tag, "i2c_master_receive() failed: id=%s err=%s", id_.c_str(),
                 esp_err_to_name(err));

        if (err == ESP_ERR_INVALID_ARG) {
            return status::StatusCode::InvalidArg;
        }
        if (err == ESP_ERR_TIMEOUT) {
            return status::StatusCode::Timeout;
        }

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace i2c
} // namespace io
} // namespace ocs
