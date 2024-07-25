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
class MDNSProvider : public core::NonCopyable<> {
public:
    struct Params {
        std::string hostname;
        std::string instance_name;
    };

    using TxtRecord = std::pair<std::string, std::string>;
    using TxtRecordList = std::vector<TxtRecord>;

    //! Initialize.
    explicit MDNSProvider(const Params& params);

    //! Start mDNS service.
    status::StatusCode start();

    //! Stop mDNS service.
    status::StatusCode stop();

    //! Add mDNS service.
    //!
    //! @notes
    //!  See common services: http://www.dns-sd.org/serviceTypes.html
    status::StatusCode add_service(const char* service, const char* proto, unsigned port);

    status::StatusCode add_service_txt_records(const char* service,
                                               const char* proto,
                                               const TxtRecordList& records);

private:
    using ProtoToRecords = std::unordered_map<std::string, TxtRecordList>;
    using ServiceToProto = std::unordered_map<std::string, ProtoToRecords>;

    ServiceToProto services_;

    const Params params_;
};

} // namespace net
} // namespace ocs
