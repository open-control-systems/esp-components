/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace storage {

class IStorage {
public:
    //! Destroy.
    virtual ~IStorage() = default;

    //! Read a key-value pair.
    //!
    //! @params
    //!  - @p key - name of value to read, 15 characters is a maximum length;
    //!  - @p value - value to read;
    //!  - @p size  - size of the value to read, in bytes.
    virtual status::StatusCode read(const char* key, void* value, unsigned size) = 0;

    //! Write a key-value pair.
    //!
    //! @params
    //!  - @p key - name of value to write, 15 characters is a maximum length;
    //!  - @p value - value to write;
    //!  - @p size  - size of the value to write, in bytes.
    virtual status::StatusCode
    write(const char* key, const void* value, unsigned size) = 0;

    //! Erase a key-value pair with the given key name.
    //!
    //! @params
    //!  - @p key - name of value to erase, 15 characters is a maximum length.
    virtual status::StatusCode erase(const char* key) = 0;
};

} // namespace storage
} // namespace ocs
