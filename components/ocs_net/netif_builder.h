#pragma once

#include "esp_netif.h"

#include <memory>

namespace ocs {
namespace net {

using NetifSharedPtr = std::shared_ptr<esp_netif_t>;

NetifSharedPtr make_netif_shared(esp_netif_t* netif);

} // namespace net
} // namespace ocs
