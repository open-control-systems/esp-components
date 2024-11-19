/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_pipeline/network/mdns_pipeline.h"

namespace ocs {
namespace pipeline {
namespace network {

MdnsPipeline::MdnsPipeline() {
    provider_.reset(new (std::nothrow) net::MdnsProvider(net::MdnsProvider::Params {
        .hostname = CONFIG_OCS_NETWORK_MDNS_HOSTNAME,
        .instance_name = CONFIG_OCS_NETWORK_MDNS_INSTANCE_NAME,
    }));
    configASSERT(provider_);
}

status::StatusCode MdnsPipeline::start() {
    return provider_->start();
}

net::MdnsProvider& MdnsPipeline::get_provider() {
    return *provider_;
}

} // namespace network
} // namespace pipeline
} // namespace ocs
