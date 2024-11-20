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
#include "ocs_net/fanout_network_handler.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/device_info.h"

namespace ocs {
namespace pipeline {
namespace network {

//! Select between STA and AP WiFi network configuration.
//!
//! @notes
//!  AP WiFi network is used by default.
class LocalNetworkPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p storage_builder to create storages for network configuration.
    //!  - @p device_info to create a unique access point SSID.
    LocalNetworkPipeline(storage::StorageBuilder& storage_builder,
                         const system::DeviceInfo& device_info);

    net::BasicNetwork& get_network();
    net::FanoutNetworkHandler& get_fanout_handler();

    //! Start the WiFi network.
    status::StatusCode start();

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

    void initialize_nework_(const system::DeviceInfo& device_info);
    void initialize_network_ap_(const system::DeviceInfo& device_info);
    void initialize_network_sta_();

    status::StatusCode start_();
    void stop_();

    static const unsigned max_ssid_size_ = 32;
    static const unsigned max_password_size_ = 64;

    static const TickType_t wait_start_interval_ = pdMS_TO_TICKS(1000 * 60 * 10);

    storage::StoragePtr storage_;

    std::unique_ptr<net::FanoutNetworkHandler> handler_;
    std::unique_ptr<net::BasicNetwork> network_;
};

} // namespace network
} // namespace pipeline
} // namespace ocs
