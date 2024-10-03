/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_control/fsm_store.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace control {

FsmStore::FsmStore(FsmBlock& block, const char* id)
    : log_tag_(id)
    , block_(block) {
}

status::StatusCode FsmStore::run() {
    block_.update();

    if (is_transit_begin_) {
        handle_transit_end_();
    }

    handle_state_();

    if (block_.is_in_transit()) {
        handle_transit_begin_();
    }

    return status::StatusCode::OK;
}

status::StatusCode FsmStore::add(FsmBlock::State state, IFsmHandler& handler) {
    state_to_handlers_[state].push_back(&handler);

    return status::StatusCode::OK;
}

void FsmStore::handle_transit_begin_() {
    ocs_logi(log_tag_.c_str(),
             "begin state changing: prev=%u curr=%u next=%u prev_dur=%lli curr_dur=%lli",
             block_.previous_state(), block_.current_state(), block_.next_state(),
             block_.previous_state_duration(), block_.current_state_duration());

    is_transit_begin_ = true;
}

void FsmStore::handle_transit_end_() {
    auto it = state_to_handlers_.find(block_.current_state());
    configASSERT(it != state_to_handlers_.end());

    for (auto& handler : it->second) {
        const auto code = handler->handle_transit();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag_.c_str(),
                     "failed to handle state transition: prev=%u curr=%u next=%u "
                     "prev_dur=%lli curr_dur=%lli code=%s",
                     block_.previous_state(), block_.current_state(), block_.next_state(),
                     block_.previous_state_duration(), block_.current_state_duration(),
                     status::code_to_str(code));
        }
    }

    const auto code = block_.transit();
    if (code != status::StatusCode::OK) {
        ocs_logw(log_tag_.c_str(),
                 "failed to transit block to the next state: prev=%u curr=%u next=%u "
                 "prev_dur=%lli curr_dur=%lli code=%s",
                 block_.previous_state(), block_.current_state(), block_.next_state(),
                 block_.previous_state_duration(), block_.current_state_duration(),
                 status::code_to_str(code));
    }

    ocs_logi(log_tag_.c_str(),
             "end state changing: prev=%u curr=%u next=%u prev_dur=%lli curr_dur=%lli",
             block_.previous_state(), block_.current_state(), block_.next_state(),
             block_.previous_state_duration(), block_.current_state_duration());

    is_transit_begin_ = false;
}

void FsmStore::handle_state_() {
    auto it = state_to_handlers_.find(block_.current_state());
    configASSERT(it != state_to_handlers_.end());

    for (auto& handler : it->second) {
        const auto code = handler->handle_state();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag_.c_str(),
                     "failed to handle state: prev=%u curr=%u next=%u prev_dur=%lli "
                     "curr_dur=%lli code=%s",
                     block_.previous_state(), block_.current_state(), block_.next_state(),
                     block_.previous_state_duration(), block_.current_state_duration(),
                     status::code_to_str(code));
        }
    }
}

} // namespace control
} // namespace ocs
