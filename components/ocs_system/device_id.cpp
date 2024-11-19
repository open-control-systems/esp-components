/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "esp_mac.h"

#include "ocs_core/log.h"
#include "ocs_security/mbedtls_sha_engine.h"
#include "ocs_security/sha_generator.h"
#include "ocs_status/code_to_str.h"
#include "ocs_system/device_id.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "device_id";

} // namespace

DeviceID::DeviceID() {
    security::MbedTlsShaEngine engine(security::IShaEngine::Algorithm::SHA1);

    security::ShaGenerator generator(engine);

    generator.add(reinterpret_cast<const uint8_t*>("open-control-systems-"),
                  strlen("open-control-systems-"));

    uint8_t mac[6];
    memset(mac, 0, sizeof(mac));
    if (const auto err = esp_efuse_mac_get_default(mac); err != ESP_OK) {
        ocs_loge(log_tag, "esp_efuse_mac_get_default(): %s", esp_err_to_name(err));
    } else {
        generator.add(mac, sizeof(mac));
    }

    generator.add(reinterpret_cast<const uint8_t*>("-device-id"), strlen("-device-id"));

    configASSERT(generator.generate() == status::StatusCode::OK);

    const auto sha = generator.get_sha();

    id_.reset(new (std::nothrow) security::sha_to_hex_str(sha.data(), sha.size()));
    configASSERT(id_);

    ocs_logi(log_tag, "device ID generated: id=%s", id_->c_str());
}

const char* DeviceID::get_id() const {
    return id_->c_str();
}

} // namespace system
} // namespace ocs
