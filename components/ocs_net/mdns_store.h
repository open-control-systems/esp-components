/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_net/imdns_driver.h"
#include "ocs_status/code.h"

namespace ocs {
namespace net {

class MdnsStore : public IMdnsDriver, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p driver to perform actual mDNS operations.
    explicit MdnsStore(IMdnsDriver& driver);

    //! Start mDNS driver.
    //!
    //! @notes
    //!  - Flush previously added services and txt records to the underlying mDNS driver.
    status::StatusCode start() override;

    //! Stop mDNS driver.
    status::StatusCode stop() override;

    //! Add mDNS service.
    status::StatusCode add_service(IMdnsDriver::Service service,
                                   IMdnsDriver::Proto proto,
                                   unsigned port) override;

    //! Add txt record for the mDNS service.
    status::StatusCode add_txt_record(IMdnsDriver::Service service,
                                      IMdnsDriver::Proto proto,
                                      const char* key,
                                      const char* value) override;

private:
    using TxtRecord = std::pair<std::string, std::string>;
    using TxtRecordList = std::vector<TxtRecord>;

    struct Node {
        unsigned port { 0 };
        TxtRecordList records;
    };

    using ProtoToNode = std::unordered_map<IMdnsDriver::Proto, Node>;
    using ServiceToProto = std::unordered_map<IMdnsDriver::Service, ProtoToNode>;

    status::StatusCode flush_();

    IMdnsDriver& driver_;

    ServiceToProto services_;
};

} // namespace net
} // namespace ocs
