/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "lwip/inet.h"

#include "ocs_net/ip_addr_to_str.h"

namespace ocs {
namespace net {

ip_addr_to_str::ip_addr_to_str(ip_addr_t addr) {
    if (!inet_ntoa_r(addr, buf_, sizeof(buf_))) {
        strcpy(buf_, "<none>");
    }
}

const char* ip_addr_to_str::c_str() const {
    return buf_;
}

} // namespace net
} // namespace ocs
