#include "esp_err.h"
#include "nvs_flash.h"

#include "ocs_storage/flash_storage.h"

namespace ocs {
namespace storage {

FlashStorage::FlashStorage() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

} // namespace storage
} // namespace ocs
