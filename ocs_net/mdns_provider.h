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

    //! Start MDNS service.
    status::StatusCode start();

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
