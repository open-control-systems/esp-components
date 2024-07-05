#pragma once

#include <mutex>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

//! Static FreeRTOS mutex.
class StaticMutex : public NonCopyable<> {
public:
    //! RAII lock.
    using Lock = std::lock_guard<StaticMutex>;

    //! Initialize.
    StaticMutex();

    //! Lock mutex.
    void lock();

    //! Unlock mutex.
    void unlock();

private:
    StaticSemaphore_t buff_;
    SemaphoreHandle_t sem_ { nullptr };
};

} // namespace core
} // namespace ocs
