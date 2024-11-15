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
#include "ocs_net/mdns_provider.h"
#include "ocs_storage/storage_builder.h"

namespace ocs {
namespace pipeline {
namespace basic {

class NetworkPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p storage_builder to create storages for network configuration.
    explicit NetworkPipeline(storage::StorageBuilder& storage_builder);

    //! Start the pipeline.
    status::StatusCode start();

    //! Return network instance.
    net::BasicNetwork& get_network();

    //! Return mDNS instance.
    net::MdnsProvider& get_mdns_provider();

private:
    void initialize_nework_();

    status::StatusCode start_();
    void stop_();

    static const unsigned max_ssid_size_ = 32;
    static const unsigned max_password_size_ = 64;

    storage::StoragePtr sta_storage_;

    std::unique_ptr<net::BasicNetwork> network_;
    std::unique_ptr<net::MdnsProvider> mdns_provider_;
};

} // namespace basic
} // namespace pipeline
} // namespace ocs
