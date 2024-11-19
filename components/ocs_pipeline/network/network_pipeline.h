/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_pipeline/network/inetwork_selector.h"
#include "ocs_status/code.h"

namespace ocs {
namespace pipeline {
namespace network {

class NetworkPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p selector to select the WiFi network.
    explicit NetworkPipeline(INetworkSelector& selector);

    //! Start the WiFi network.
    status::StatusCode start();

private:
    status::StatusCode start_();
    void stop_();

    static const TickType_t wait_start_interval_ = pdMS_TO_TICKS(1000 * 60 * 10);

    INetworkSelector& selector_;
};

} // namespace network
} // namespace pipeline
} // namespace ocs
