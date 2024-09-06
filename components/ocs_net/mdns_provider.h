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
#include "ocs_status/code.h"

namespace ocs {
namespace net {

//! Provides local network service.
//!
//! @references
//!  - https://datatracker.ietf.org/doc/html/rfc2782
//!  - https://datatracker.ietf.org/doc/html/rfc6335
//!  - https://www.ietf.org/rfc/rfc6763.txt
class MdnsProvider : public core::NonCopyable<> {
public:
    //! Known mDNS services.
    //!
    //! @references
    //!  See common services: http://www.dns-sd.org/serviceTypes.html
    enum class Service {
        Http,
    };

    //! Known transport protocols.
    enum class Proto {
        //! For application protocols that run over TCP.
        Tcp,
    };

    struct Params {
        std::string hostname;
        std::string instance_name;
    };

    using TxtRecord = std::pair<std::string, std::string>;
    using TxtRecordList = std::vector<TxtRecord>;

    //! Initialize.
    explicit MdnsProvider(const Params& params);

    //! Start mDNS service.
    status::StatusCode start();

    //! Stop mDNS service.
    status::StatusCode stop();

    //! Add mDNS service.
    status::StatusCode add_service(Service service, Proto proto, unsigned port);

    //! Add txt records for the mDNS service.
    void add_txt_records(Service service, Proto proto, const TxtRecordList& records);

    //! Flush previously added txt records to the underlying mDNS driver.
    status::StatusCode flush_txt_records();

private:
    using ProtoToRecords = std::unordered_map<Proto, TxtRecordList>;
    using ServiceToProto = std::unordered_map<Service, ProtoToRecords>;

    ServiceToProto services_;

    const Params params_;
};

} // namespace net
} // namespace ocs
