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
namespace core {

//! Stream reading process.
//!
//! @example
//!  status::StatusCode read_data(IStreamReader& reader, void* data, unsigned size) {
//!     if (const auto code = reader.begin(); code != status::StatusCode::OK) {
//!         return code;
//!     }
//!
//!     while (true) {
//!         const auto code = reader.read(data, size);
//!         if (code != status::StatusCode::OK) {
//!             if (code != status::StatusCode::NoData) {
//!                 // Optionally it's possible to cancel the process and retry it later.
//!                 return reader.cancel();
//!             }
//!
//!             break;
//!         }
//!
//!         // Handle data.
//!         // size variable should contain the actual number of bytes that were read.
//!     }
//!
//!     return reader.end();
//!  }
class IStreamReader {
public:
    //! Destroy.
    virtual ~IStreamReader() = default;

    //! Begin stream reading.
    //!
    //! @notes
    //!  It should be possible to begin the process any number of times.
    virtual status::StatusCode begin() = 0;

    //! End stream reading.
    //!
    //! @notes
    //!  It should be possible to end the process any number of times.
    virtual status::StatusCode end() = 0;

    //! Cancel stream reading.
    //!
    //! @notes
    //!  - It should be possible to cancel the process any number of times.
    //!  - It should be possible to cancel the process at any time.
    //!  - If the process is canceled it should be started from the beginning.
    virtual status::StatusCode cancel() = 0;

    //! Read @p size bytes to @p data.
    //!
    //! @notes
    //!  Reading is only allowed after the process was started correctly via begin() call.
    virtual status::StatusCode read(void* data, unsigned& size) = 0;
};

} // namespace core
} // namespace ocs
