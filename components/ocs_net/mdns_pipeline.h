/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_net/imdns_driver.h"

namespace ocs {
namespace net {

class MdnsPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    explicit MdnsPipeline();

    //! Start the pipeline.
    status::StatusCode start();

    //! Get the underlying mDNS driver.
    IMdnsDriver& get_driver();

private:
    std::unique_ptr<IMdnsDriver> driver_;
    std::unique_ptr<IMdnsDriver> store_;
};

} // namespace net
} // namespace ocs
