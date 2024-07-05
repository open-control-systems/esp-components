#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/static_mutex.h"

namespace ocs {
namespace core {

StaticMutex::StaticMutex() {
    memset(&buff_, 0, sizeof(buff_));

    sem_ = xSemaphoreCreateMutexStatic(&buff_);
    configASSERT(sem_);
}

void StaticMutex::lock() {
    xSemaphoreTake(sem_, portMAX_DELAY);
}

void StaticMutex::unlock() {
    xSemaphoreGive(sem_);
}

} // namespace core
} // namespace ocs
