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

namespace ocs {
namespace pipeline {
namespace basic {

class NetworkPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @remarks
    //!  NVS should be initialized.
    NetworkPipeline();

    //! Start the pipeline.
    status::StatusCode start();

    //! Return network instance.
    net::BasicNetwork& get_network();

    //! Return mDNS instance.
    net::MdnsProvider& get_mdns_provider();

private:
    status::StatusCode start_();
    void stop_();

    std::unique_ptr<net::BasicNetwork> network_;
    std::unique_ptr<net::MdnsProvider> mdns_provider_;
};

} // namespace basic
} // namespace pipeline
} // namespace ocs
