/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_iot/ijson_formatter.h"
#include "ocs_net/basic_network.h"

namespace ocs {
namespace iot {

class NetworkJsonFormatter : public iot::IJsonFormatter, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p network to read the network data.
    explicit NetworkJsonFormatter(net::BasicNetwork& network);

    //! Format the network data in @p json.
    void format(cJSON* json) override;

private:
    void format_ap_info_(cJSON* json);
    void format_ip_addr_(cJSON* json);

    net::BasicNetwork& network_;
};

} // namespace iot
} // namespace ocs
