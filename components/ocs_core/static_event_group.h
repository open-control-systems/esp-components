#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

//! Static FreeRTOS event group.
class StaticEventGroup : public NonCopyable<> {
public:
    //! Initialize.
    StaticEventGroup();

    //! Return event group handler.
    EventGroupHandle_t get() const;

private:
    StaticEventGroup_t buf_;
    EventGroupHandle_t group_;
};

} // namespace core
} // namespace ocs
