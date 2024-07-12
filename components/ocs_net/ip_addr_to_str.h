/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "lwip/ip_addr.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace net {

class ip_addr_to_str : public core::NonCopyable<> {
public:
    //! Initialize.
    explicit ip_addr_to_str(ip_addr_t addr);

    //! Return formatted address.
    const char* c_str() const;

private:
    char buf_[IPADDR_STRLEN_MAX];
};

} // namespace net
} // namespace ocs
