/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_storage/ns_storage.h"

namespace ocs {
namespace storage {

namespace {

const char* log_tag = "ns-storage";

} // namespace

NsStorage::NsStorage(const char* ns) {
    memset(ns_, 0, sizeof(ns_));
    strncpy(ns_, ns, std::min(bufsize_, strlen(ns)));
}

status::StatusCode NsStorage::read(const char* key, void* value, size_t size) {
    configASSERT(key);
    configASSERT(value);
    configASSERT(size);

    auto [handle, code] = open_(NVS_READONLY);
    if (code == status::StatusCode::OK) {
        code = read_(handle, key, value, size);
        nvs_close(handle);
    }

    return code;
}

status::StatusCode NsStorage::write(const char* key, const void* value, size_t size) {
    configASSERT(key);
    configASSERT(value);
    configASSERT(size);

    auto [handle, code] = open_(NVS_READWRITE);
    if (code == status::StatusCode::OK) {
        code = write_(handle, key, value, size);
        nvs_close(handle);
    }

    return code;
}

status::StatusCode NsStorage::erase(const char* key) {
    configASSERT(key);

    auto [handle, code] = open_(NVS_READWRITE);
    if (code == status::StatusCode::OK) {
        code = erase_(handle, key);
        nvs_close(handle);
    }

    return code;
}

std::pair<nvs_handle_t, status::StatusCode> NsStorage::open_(nvs_open_mode_t mode) {
    nvs_handle_t handle = 0;

    const auto err = nvs_open(ns_, mode, &handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return { 0, status::StatusCode::NoData };
    }

    if (err != ESP_OK) {
        ocs_loge(log_tag, "nvs_open(): %s", esp_err_to_name(err));
        return { 0, status::StatusCode::Error };
    }

    return { handle, status::StatusCode::OK };
}

status::StatusCode
NsStorage::read_(nvs_handle_t handle, const char* key, void* value, size_t size) {
    const auto err = nvs_get_blob(handle, key, value, &size);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return status::StatusCode::NoData;
    }

    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to read: nvs_get_blob(): key=%s err=%s", key,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode
NsStorage::write_(nvs_handle_t handle, const char* key, const void* value, size_t size) {
    auto err = nvs_set_blob(handle, key, value, size);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to write: nvs_set_blob(): key=%s err=%s", key,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to write: nvs_commit(): key=%s err=%s", key,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode NsStorage::erase_(nvs_handle_t handle, const char* key) {
    auto err = nvs_erase_key(handle, key);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return status::StatusCode::NoData;
    }

    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to erase: nvs_erase_key(): key=%s err=%s", key,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to erase: nvs_commit(): key=%s err=%s", key,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace storage
} // namespace ocs
