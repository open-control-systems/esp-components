/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_pipeline/jsonfmt/ap_network_formatter.h"
#include "ocs_pipeline/network/local_network_json_pipeline.h"

namespace ocs {
namespace pipeline {
namespace network {

LocalNetworkJsonPipeline::LocalNetworkJsonPipeline(
    storage::StorageBuilder& storage_builder,
    fmt::json::FanoutFormatter& registraton_formatter,
    const system::DeviceInfo& device_info) {
    network_pipeline_.reset(new (std::nothrow)
                                LocalNetworkPipeline(storage_builder, device_info));
    configASSERT(network_pipeline_);

    network_formatter_.reset(
        new (std::nothrow) jsonfmt::ApNetworkFormatter(network_pipeline_->get_network()));
    configASSERT(network_formatter_);

    registraton_formatter.add(*network_formatter_);
}

LocalNetworkPipeline& LocalNetworkJsonPipeline::get_network_pipeline() {
    return *network_pipeline_;
}

} // namespace network
} // namespace pipeline
} // namespace ocs
