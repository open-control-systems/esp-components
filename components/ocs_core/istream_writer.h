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

//! Stream writing process.
//!
//! @example
//!  status::StatusCode write(IStreamWriter& writer, const void* data, unsigned size) {
//!     if (const auto code = writer.begin(); code != status::StatusCode::OK) {
//!         return code;
//!     }
//!
//!     const auto code = writer.write(data, size);
//!     if (code != status::StatusCode::OK) {
//!         // Optionally it's possible to cancel the process and retry it later.
//!         return writer.cancel();
//!     }
//!
//!     // Data has been successfully written.
//!
//!     return writer.end();
//!  }
class IStreamWriter {
public:
    //! Destroy.
    virtual ~IStreamWriter() = default;

    //! Begin stream writing.
    //!
    //! @notes
    //!  It should be possible to begin the process any number of times.
    virtual status::StatusCode begin() = 0;

    //! End stream writing.
    //!
    //! @notes
    //!  It should be possible to end the process any number of times.
    virtual status::StatusCode end() = 0;

    //! Cancel stream writing.
    //!
    //! @notes
    //!  - It should be possible to cancel the process any number of times.
    //!  - It should be possible to cancel the process at any time.
    //!  - If the process is canceled it should be started from the beginning.
    virtual status::StatusCode cancel() = 0;

    //! Write @p size bytes of @p data.
    //!
    //! @notes
    //!  Writing is only allowed after the process was started correctly via begin() call.
    virtual status::StatusCode write(const void* data, unsigned size) = 0;
};

} // namespace core
} // namespace ocs
