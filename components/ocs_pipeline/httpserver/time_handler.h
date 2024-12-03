/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <ctime>

#include "ocs_core/noncopyable.h"
#include "ocs_http/server.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class TimeHandler : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register HTTP endpoint.
    //!  - @p start_point - point in time begin with the system time is valid.
    TimeHandler(http::Server& server, time_t start_point);
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
