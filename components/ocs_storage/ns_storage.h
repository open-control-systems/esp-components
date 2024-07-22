/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <utility>

#include "nvs.h"
#include "nvs_flash.h"

#include "ocs_core/noncopyable.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace storage {

class NsStorage : public IStorage, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p ns - NVS namespace.
    //!
    //! @remarks
    //!  NVS should be initialized.
    explicit NsStorage(const char* ns);

    //! Read data from the configured namespace.
    status::StatusCode read(const char* key, void* value, size_t size) override;

    //! Write data to the configured namespace.
    status::StatusCode write(const char* key, const void* value, size_t size) override;

    //! Erase data from the configured namespace.
    status::StatusCode erase(const char* key) override;

private:
    std::pair<nvs_handle_t, status::StatusCode> open_(nvs_open_mode_t mode);

    status::StatusCode
    read_(nvs_handle_t handle, const char* key, void* value, size_t size);

    status::StatusCode
    write_(nvs_handle_t handle, const char* key, const void* value, size_t size);

    status::StatusCode erase_(nvs_handle_t handle, const char* key);

    static const constexpr unsigned bufsize_ = NVS_KEY_NAME_MAX_SIZE - 1;

    char ns_[bufsize_ + 1];
};

} // namespace storage
} // namespace ocs
