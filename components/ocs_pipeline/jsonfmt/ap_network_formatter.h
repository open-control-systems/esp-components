/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"
#include "ocs_net/iap_network.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class ApNetworkFormatter : public fmt::json::IFormatter, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p network to read the network characteristics.
    explicit ApNetworkFormatter(net::IApNetwork& network);

    //! Format the network data in @p json.
    status::StatusCode format(cJSON* json) override;

private:
    net::IApNetwork& network_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
