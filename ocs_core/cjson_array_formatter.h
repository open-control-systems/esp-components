#pragma once

#include "cJSON.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

class cJSONArrayFormatter : public NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @remarks
    //!  - @p json should be an array.
    explicit cJSONArrayFormatter(cJSON* json);

    //! Append 16-bit number stored to json.
    bool append_uint16(uint16_t value);

private:
    cJSON* json_ { nullptr };
};

} // namespace core
} // namespace ocs
