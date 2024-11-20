/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace net {

//! mDNS Driver.
//!
//! @references
//!  - https://datatracker.ietf.org/doc/html/rfc2782
//!  - https://datatracker.ietf.org/doc/html/rfc6335
//!  - https://www.ietf.org/rfc/rfc6763.txt
class IMdnsDriver {
public:
    //! Known mDNS services.
    //!
    //! @references
    //!  See common services: http://www.dns-sd.org/serviceTypes.html
    enum class Service {
        //! HTTP mDNS service.
        Http,
    };

    //! Known transport protocols.
    enum class Proto {
        //! For application protocols that run over TCP.
        Tcp,
    };

    //! Destroy.
    virtual ~IMdnsDriver() = default;

    //! Start mDNS driver.
    virtual status::StatusCode start() = 0;

    //! Stop mDNS driver.
    virtual status::StatusCode stop() = 0;

    //! Register a new mDNS service.
    virtual status::StatusCode
    add_service(Service service, Proto proto, unsigned port) = 0;

    //! Add a txt record for the mDNS service.
    virtual status::StatusCode
    add_txt_record(Service service, Proto proto, const char* key, const char* value) = 0;
};

} // namespace net
} // namespace ocs
