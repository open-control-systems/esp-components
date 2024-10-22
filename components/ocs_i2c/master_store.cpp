/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "driver/i2c_master.h"
#include "driver/i2c_types.h"

#include "ocs_core/log.h"
#include "ocs_i2c/master_store.h"
#include "ocs_i2c/master_transceiver.h"

namespace ocs {
namespace i2c {

namespace {

const char* log_tag = "i2c_master_store";

} // namespace

MasterStore::MasterStore(MasterStore::Params params) {
    i2c_master_bus_config_t config;
    memset(&config, 0, sizeof(config));

    config.clk_source = I2C_CLK_SRC_DEFAULT;
    config.i2c_port = I2C_NUM_0;
    config.sda_io_num = params.sda;
    config.scl_io_num = params.scl;
    config.glitch_ignore_cnt = 7;
    config.flags.enable_internal_pullup = true;

    ESP_ERROR_CHECK(i2c_new_master_bus(&config, &handle_));
}

IStore::ITransceiverPtr MasterStore::add(const char* id,
                                         IStore::AddressLength len,
                                         IStore::Address addr,
                                         IStore::TransferSpeed speed) {
    i2c_device_config_t config;
    memset(&config, 0, sizeof(config));

#if SOC_I2C_SUPPORT_10BIT_ADDR
    if (len == IStore::AddressLength::Bit_10) {
        config.dev_addr_length = I2C_ADDR_BIT_LEN_10;
    } else {
        config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    }
#else  // !SOC_I2C_SUPPORT_10BIT_ADDR
    configASSERT(len == IStore::AddressLength::Bit_7);
    config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
#endif // SOC_I2C_SUPPORT_10BIT_ADDR

    config.device_address = addr;

    if (speed == IStore::TransferSpeed::Fast) {
        config.scl_speed_hz = 400000;
    } else {
        config.scl_speed_hz = 100000;
    }

    i2c_master_dev_handle_t device = nullptr;

    auto err = i2c_master_bus_add_device(handle_, &config, &device);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "i2c_master_bus_add_device() failed: id=%s err=%s", id,
                 esp_err_to_name(err));

        return nullptr;
    }

    auto device_ptr = MasterTransceiver::make_device_shared(device);
    if (!device_ptr) {
        ocs_loge(log_tag, "failed to allocate memory for I2C device: id=%s", id);

        return nullptr;
    }

    IStore::ITransceiverPtr transceiver(new (std::nothrow)
                                            MasterTransceiver(device_ptr, id));
    if (!transceiver) {
        ocs_loge(log_tag, "failed to allocate memory for I2C transceiver: id=%s", id);

        return nullptr;
    }

    return transceiver;
}

MasterStore::~MasterStore() {
    ESP_ERROR_CHECK(i2c_del_master_bus(handle_));
}

} // namespace i2c
} // namespace ocs
