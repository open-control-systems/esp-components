/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/mdns_store.h"
#include "ocs_core/log.h"
#include "ocs_net/mdns_to_str.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "mdns_store";

} // namespace

MdnsStore::MdnsStore(IMdnsDriver& driver)
    : driver_(driver) {
}

status::StatusCode MdnsStore::start() {
    auto code = driver_.start();
    if (code == status::StatusCode::OK) {
        code = flush_();
    }

    return code;
}

status::StatusCode MdnsStore::stop() {
    return driver_.stop();
}

status::StatusCode MdnsStore::add_service(IMdnsDriver::Service service,
                                          IMdnsDriver::Proto proto,
                                          unsigned port) {
    services_[service][proto].port = port;

    return status::StatusCode::OK;
}

status::StatusCode MdnsStore::add_txt_record(MdnsStore::Service service,
                                             MdnsStore::Proto proto,
                                             const char* key,
                                             const char* value) {
    services_[service][proto].records.push_back(std::make_pair(key, value));

    return status::StatusCode::OK;
}

status::StatusCode MdnsStore::flush_() {
    for (const auto& [service, proto_to_node] : services_) {
        for (const auto& [proto, node] : proto_to_node) {
            const auto code = driver_.add_service(service, proto, node.port);
            if (code != status::StatusCode::OK) {
                ocs_loge(log_tag,
                         "failed to add service: service=%s proto=%s port=%u code=%s",
                         mdns_service_to_str(service), mdns_proto_to_str(proto),
                         node.port, status::code_to_str(code));

                continue;
            }

            for (const auto& record : node.records) {
                const auto code = driver_.add_txt_record(
                    service, proto, record.first.c_str(), record.second.c_str());
                if (code != status::StatusCode::OK) {
                    ocs_loge(log_tag,
                             "failed to flush txt record: service=%s proto=%s key=%s "
                             "value=%s code=%s",
                             mdns_service_to_str(service), mdns_proto_to_str(proto),
                             record.first.c_str(), record.second.c_str(),
                             status::code_to_str(code));
                }
            }
        }
    }

    return status::StatusCode::OK;
}

} // namespace net
} // namespace ocs
