/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/mdns_to_str.h"

namespace ocs {
namespace net {

const char* mdns_service_to_str(IMdnsDriver::Service service) {
    switch (service) {
    case IMdnsDriver::Service::Http:
        return "_http";

    default:
        break;
    }

    return "<none>";
}

const char* mdns_proto_to_str(IMdnsDriver::Proto proto) {
    switch (proto) {
    case IMdnsDriver::Proto::Tcp:
        return "_tcp";

    default:
        break;
    }

    return "<none>";
}

} // namespace net
} // namespace ocs
