/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_net/imdns_driver.h"

namespace ocs {
namespace net {

class DefaultMdnsDriver : public IMdnsDriver, public core::NonCopyable<> {
public:
    //! Initialize.
    DefaultMdnsDriver(const char* hostname, const char* instance_name);

    //! Start mDNS driver.
    status::StatusCode start() override;

    //! Stop mDNS driver.
    status::StatusCode stop() override;

    //! Add mDNS service.
    status::StatusCode add_service(Service service, Proto proto, unsigned port) override;

    //! Add txt record for the mDNS service.
    status::StatusCode add_txt_record(Service service,
                                      Proto proto,
                                      const char* key,
                                      const char* value) override;

private:
    std::string hostname_;
    std::string instance_name_;
};

} // namespace net
} // namespace ocs
