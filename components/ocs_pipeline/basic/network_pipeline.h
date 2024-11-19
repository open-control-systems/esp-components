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
#include "ocs_net/basic_network.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/device_id.h"

namespace ocs {
namespace pipeline {
namespace basic {

class NetworkPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p storage_builder to create storages for network configuration.
    //!  - @p device_id to create a unique access point SSID.
    NetworkPipeline(storage::StorageBuilder& storage_builder,
                    system::DeviceID& device_id);

    //! Start the pipeline.
    status::StatusCode start();

    //! Return network instance.
    net::BasicNetwork& get_network();

private:
    enum class NetworkType {
        //! WiFi access point mode.
        Ap,

        //! WiFi station mode.
        Sta,

        //! Last invalid type.
        Last,
    };

    status::StatusCode read_network_type_(NetworkType& type);

    void initialize_nework_(system::DeviceID& device_id);
    void initialize_network_ap_(system::DeviceID& device_id);
    void initialize_network_sta_();

    status::StatusCode start_();
    void stop_();

    static const TickType_t wait_start_interval_ = pdMS_TO_TICKS(1000 * 60 * 10);

    static const unsigned max_ssid_size_ = 32;
    static const unsigned max_password_size_ = 64;

    storage::StoragePtr storage_;

    std::unique_ptr<net::BasicNetwork> network_;
};

} // namespace basic
} // namespace pipeline
} // namespace ocs
