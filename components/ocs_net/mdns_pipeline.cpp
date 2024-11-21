/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_net/default_mdns_driver.h"
#include "ocs_net/mdns_pipeline.h"
#include "ocs_net/mdns_store.h"

namespace ocs {
namespace net {

MdnsPipeline::MdnsPipeline(const char* hostname, const char* instance_name) {
    driver_.reset(new (std::nothrow) DefaultMdnsDriver(hostname, instance_name));
    configASSERT(driver_);

    store_.reset(new (std::nothrow) MdnsStore(*driver_));
    configASSERT(store_);
}

status::StatusCode MdnsPipeline::start() {
    return store_->start();
}

IMdnsDriver& MdnsPipeline::get_driver() {
    return *store_;
}

} // namespace net
} // namespace ocs
