/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "mdns.h"

#include "ocs_core/log.h"
#include "ocs_net/default_mdns_driver.h"
#include "ocs_net/mdns_to_str.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "default_mdns_driver";

} // namespace

DefaultMdnsDriver::DefaultMdnsDriver(const char* hostname, const char* instance_name)
    : hostname_(hostname)
    , instance_name_(instance_name) {
}

status::StatusCode DefaultMdnsDriver::start() {
    auto err = mdns_init();
    if (err != ESP_OK) {
        ocs_loge(log_tag, "mdns_init(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    err = mdns_hostname_set(hostname_.c_str());
    if (err != ESP_OK) {
        ocs_loge(log_tag, "mdns_hostname_set(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    err = mdns_instance_name_set(instance_name_.c_str());
    if (err != ESP_OK) {
        ocs_loge(log_tag, "mdns_instance_name_set(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode DefaultMdnsDriver::stop() {
    mdns_free();
    return status::StatusCode::OK;
}

status::StatusCode
DefaultMdnsDriver::add_service(Service service, Proto proto, unsigned port) {
    const auto err = mdns_service_add(nullptr, mdns_service_to_str(service),
                                      mdns_proto_to_str(proto), port, nullptr, 0);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to add service: service=%s proto=%s port=%u",
                 mdns_service_to_str(service), mdns_proto_to_str(proto), port);

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode DefaultMdnsDriver::add_txt_record(Service service,
                                                     Proto proto,
                                                     const char* key,
                                                     const char* value) {
    const auto err = mdns_service_txt_item_set(mdns_service_to_str(service),
                                               mdns_proto_to_str(proto), key, value);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "mdns_service_txt_item_set(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace net
} // namespace ocs
