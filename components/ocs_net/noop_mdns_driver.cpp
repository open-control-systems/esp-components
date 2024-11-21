/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/noop_mdns_driver.h"

namespace ocs {
namespace net {

status::StatusCode NoopMdnsDriver::start() {
    return status::StatusCode::OK;
}

status::StatusCode NoopMdnsDriver::stop() {
    return status::StatusCode::OK;
}

status::StatusCode
NoopMdnsDriver::add_service(Service service, Proto proto, unsigned port) {
    return status::StatusCode::OK;
}

status::StatusCode NoopMdnsDriver::add_txt_record(Service service,
                                                  Proto proto,
                                                  const char* key,
                                                  const char* value) {
    return status::StatusCode::OK;
}

} // namespace net
} // namespace ocs
