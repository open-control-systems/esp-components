#pragma once

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace storage {

class FlashStorage : public core::NonCopyable<> {
public:
    //! Initialize NVS.
    FlashStorage();
};

} // namespace storage
} // namespace ocs

