/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_net/imdns_driver.h"

namespace ocs {
namespace net {

//! Return string representation of the mDNS service.
const char* mdns_service_to_str(IMdnsDriver::Service service);

//! Return string representation of the mDNS transport protocol.
const char* mdns_proto_to_str(IMdnsDriver::Proto proto);

} // namespace net
} // namespace ocs
