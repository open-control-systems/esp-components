/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "mdns.h"

#include "ocs_net/mdns_provider.h"

namespace ocs {
namespace net {

MDNSProvider::MDNSProvider(const Params& params)
    : params_(params) {
}

status::StatusCode MDNSProvider::start() {
    ESP_ERROR_CHECK(mdns_init());
    ESP_ERROR_CHECK(mdns_hostname_set(params_.hostname.c_str()));
    ESP_ERROR_CHECK(mdns_instance_name_set(params_.instance_name.c_str()));

    return status::StatusCode::OK;
}

status::StatusCode
MDNSProvider::add_service(const char* service, const char* proto, unsigned port) {
    ESP_ERROR_CHECK(mdns_service_add(nullptr, service, proto, port, nullptr, 0));

    return status::StatusCode::OK;
}

status::StatusCode MDNSProvider::add_service_txt_records(const char* service,
                                                         const char* proto,
                                                         const TxtRecordList& records) {
    auto& existing_records = services_[service][proto];
    for (const auto& record : records) {
        existing_records.emplace_back(record);
    }

    for (const auto& record : records) {
        ESP_ERROR_CHECK(mdns_service_txt_item_set(service, proto, record.first.c_str(),
                                                  record.second.c_str()));
    }

    return status::StatusCode::OK;
}

} // namespace net
} // namespace ocs
