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
#include "ocs_net/mdns_provider.h"

namespace ocs {
namespace net {

class MdnsPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    explicit MdnsPipeline();

    //! Start pipeline.
    status::StatusCode start();

    //! Get the underlying mDNS provider.
    MdnsProvider& get_provider();

private:
    std::unique_ptr<MdnsProvider> provider_;
};

} // namespace net
} // namespace ocs
