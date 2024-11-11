/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_io/i2c/master_store.h"
#include "ocs_io/i2c/master_store_pipeline.h"
#include "ocs_status/code_to_str.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace io {
namespace i2c {

namespace {

const char* log_tag = "i2c_master_store_pipeline";

} // namespace

MasterStorePipeline::MasterStorePipeline(IStore::Params params) {
    store_.reset(new (std::nothrow) MasterStore(params));
    configASSERT(store_);

    transceiver_ = store_->add("bus_transceiver", IStore::AddressLength::Bit_7, 0x00,
                               IStore::TransferSpeed::Fast);
    configASSERT(transceiver_);

    if (const auto code = reset_(); code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to reset all devices on the bus: %s",
                 status::code_to_str(code));
    } else {
        ocs_logi(log_tag, "all devices were reset");
    }
}

IStore& MasterStorePipeline::get_store() {
    return *store_;
}

status::StatusCode MasterStorePipeline::reset_() {
    const uint8_t command = 0x06;

    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_->send(&command, sizeof(command), core::Duration::second * 5));

    return status::StatusCode::OK;
}

} // namespace i2c
} // namespace io
} // namespace ocs
