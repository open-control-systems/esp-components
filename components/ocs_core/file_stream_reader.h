/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "ocs_core/istream_reader.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

class FileStreamReader : public IStreamReader, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p path - file path.
    explicit FileStreamReader(const char* path);

    //! Ensure the file reading is finished.
    ~FileStreamReader();

    //! Begin file reading.
    status::StatusCode begin() override;

    //! End file reading.
    //!
    //! @remarks
    //!  Ensure that file reading is ended to prevent file descriptor leakage.
    status::StatusCode end() override;

    //! Start file reading from the beginning of the file.
    status::StatusCode cancel() override;

    //! Read up to @p size bytes from file to @p data.
    status::StatusCode read(void* data, unsigned& size) override;

private:
    const std::string path_;

    FILE* file_ { nullptr };
};

} // namespace core
} // namespace ocs
