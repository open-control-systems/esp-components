/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "esp_log.h"
#include "mdns.h"

#include "ocs_net/mdns_provider.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "mDNS-provider";

static const char* service_to_str(MdnsProvider::Service service) {
    switch (service) {
    case MdnsProvider::Service::Http:
        return "_http";

    default:
        break;
    }

    return "<none>";
}

static const char* proto_to_str(MdnsProvider::Proto proto) {
    switch (proto) {
    case MdnsProvider::Proto::Tcp:
        return "_tcp";

    default:
        break;
    }

    return "<none>";
}

} // namespace

MdnsProvider::MdnsProvider(const Params& params)
    : params_(params) {
}

status::StatusCode MdnsProvider::start() {
    ESP_ERROR_CHECK(mdns_init());
    ESP_ERROR_CHECK(mdns_hostname_set(params_.hostname.c_str()));
    ESP_ERROR_CHECK(mdns_instance_name_set(params_.instance_name.c_str()));

    return status::StatusCode::OK;
}

status::StatusCode MdnsProvider::stop() {
    mdns_free();
    return status::StatusCode::OK;
}

status::StatusCode MdnsProvider::add_service(MdnsProvider::Service service,
                                             MdnsProvider::Proto proto,
                                             unsigned port) {
    const auto err = mdns_service_add(nullptr, service_to_str(service),
                                      proto_to_str(proto), port, nullptr, 0);
    if (err != ESP_OK) {
        ESP_LOGE(log_tag, "failed to add service: service=%s proto=%s port=%u",
                 service_to_str(service), proto_to_str(proto), port);

        return status::StatusCode::InvalidArg;
    }

    return status::StatusCode::OK;
}

void MdnsProvider::add_txt_records(MdnsProvider::Service service,
                                   MdnsProvider::Proto proto,
                                   const TxtRecordList& records) {
    auto& existing_records = services_[service][proto];
    for (const auto& record : records) {
        existing_records.emplace_back(record);
    }
}

status::StatusCode MdnsProvider::flush_txt_records() {
    for (const auto& [service, proto_to_records] : services_) {
        for (const auto& [proto, records] : proto_to_records) {
            for (const auto& record : records) {
                const auto err = mdns_service_txt_item_set(
                    service_to_str(service), proto_to_str(proto), record.first.c_str(),
                    record.second.c_str());
                if (err != ESP_OK) {
                    ESP_LOGE(log_tag,
                             "failed to flush txt record: service=%s proto=%s key=%s "
                             "value=%s",
                             service_to_str(service), proto_to_str(proto),
                             record.first.c_str(), record.second.c_str());
                }
            }
        }
    }

    return status::StatusCode::OK;
}

} // namespace net
} // namespace ocs
