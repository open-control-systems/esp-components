/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/network/network_pipeline.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace pipeline {
namespace network {

namespace {

const char* log_tag = "network_pipeline";

} // namespace

NetworkPipeline::NetworkPipeline(INetworkSelector& selector)
    : selector_(selector) {
}

status::StatusCode NetworkPipeline::start() {
    const auto code = start_();
    if (code != status::StatusCode::OK) {
        stop_();
    }

    return code;
}

status::StatusCode NetworkPipeline::start_() {
    auto code = selector_.get_network().start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start network: %s", status::code_to_str(code));

        return code;
    }

    code = selector_.get_network().wait(wait_start_interval_);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to wait for network: %s", status::code_to_str(code));

        return code;
    }

    return status::StatusCode::OK;
}

void NetworkPipeline::stop_() {
    const auto code = selector_.get_network().stop();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to stop network: %s", status::code_to_str(code));
    }
}

} // namespace network
} // namespace pipeline
} // namespace ocs
