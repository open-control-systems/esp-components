/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_control/fsm_block_pipeline.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace control {

FsmBlockPipeline::FsmBlockPipeline(core::IClock& clock,
                                   system::FanoutRebootHandler& reboot_handler,
                                   scheduler::ITaskScheduler& task_scheduler,
                                   storage::StorageBuilder& storage_builder,
                                   const char* id,
                                   FsmBlockPipeline::Params params)
    : log_tag_(std::string(id) + "-block-pipeline")
    , block_id_(std::string(id) + "-block")
    , storage_id_(std::string(id) + "-nvs") {
    configASSERT(params.state_save_interval);
    configASSERT(params.state_interval_resolution);

    storage_ = storage_builder.make(storage_id_.c_str());
    configASSERT(storage_);

    block_.reset(new (std::nothrow) FsmBlock(
        clock, *storage_, params.state_interval_resolution, block_id_.c_str()));
    configASSERT(block_);

    reboot_handler.add(*block_);

    configASSERT(
        task_scheduler.add(*block_, block_id_.c_str(), params.state_save_interval)
        == status::StatusCode::OK);
}

FsmBlock& FsmBlockPipeline::get_block() {
    return *block_;
}

} // namespace control
} // namespace ocs
