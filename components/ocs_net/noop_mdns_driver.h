/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_net/imdns_driver.h"

namespace ocs {
namespace net {

class NoopMdnsDriver : public IMdnsDriver, public core::NonCopyable<> {
public:
    //! Initialize.
    NoopMdnsDriver(const char* hostname, const char* instance_name);

    //! Non-operational.
    status::StatusCode start() override;

    //! Non-operational.
    status::StatusCode stop() override;

    //! Non-operational.
    status::StatusCode add_service(Service service, Proto proto, unsigned port) override;

    //! Non-operational.
    status::StatusCode add_txt_record(Service service,
                                      Proto proto,
                                      const char* key,
                                      const char* value) override;
};

} // namespace net
} // namespace ocs
