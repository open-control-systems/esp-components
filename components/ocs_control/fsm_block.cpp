/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_control/fsm_block.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace control {

FsmBlock::FsmBlock(core::IClock& clock,
                   storage::IStorage& storage,
                   core::microseconds_t resolution,
                   const char* id)
    : log_tag_(id)
    , resolution_(resolution)
    , clock_(clock)
    , storage_(storage) {
    configASSERT(resolution_);

    const auto code = read_();
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag_.c_str(), "failed to restore block at startup: %s",
                     status::code_to_str(code));
        }
    } else {
        ocs_logi(log_tag_.c_str(),
                 "restore block at startup: prev=%u curr=%u next=%u prev_dur=%lli "
                 "curr_dur=%lli",
                 prev_state_, curr_state_, next_state_, prev_state_dur_, curr_state_dur_);

        saved_ts_ = curr_state_dur_;
    }
}

void FsmBlock::handle_reboot() {
    const auto code = write_();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag_.c_str(), "failed to save block on reboot: %s",
                 status::code_to_str(code));
    }
}

status::StatusCode FsmBlock::run() {
    const auto code = write_();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag_.c_str(), "failed to save block on run: %s",
                 status::code_to_str(code));
    }

    return code;
}

FsmBlock::State FsmBlock::previous_state() const {
    return prev_state_;
}

FsmBlock::State FsmBlock::current_state() const {
    return curr_state_;
}

FsmBlock::State FsmBlock::next_state() const {
    return next_state_;
}

core::microseconds_t FsmBlock::previous_state_duration() const {
    return prev_state_dur_;
}

core::microseconds_t FsmBlock::current_state_duration() const {
    return curr_state_dur_;
}

uint64_t FsmBlock::write_count() const {
    return write_count_;
}

bool FsmBlock::is_in_transit() const {
    return next_state_ && curr_state_ != next_state_;
}

status::StatusCode FsmBlock::transit() {
    prev_state_ = curr_state_;
    prev_state_dur_ = curr_state_dur_;

    curr_state_ = next_state_;
    curr_state_dur_ = 0;

    next_state_ = 0;

    start_ts_ = clock_.now();
    saved_ts_ = 0;

    return write_();
}

void FsmBlock::update() {
    const auto passed_ts = (clock_.now() - start_ts_) / resolution_;

    curr_state_dur_ = saved_ts_ + passed_ts;
}

void FsmBlock::set_next(State state) {
    next_state_ = state;
}

status::StatusCode FsmBlock::write_() {
    ++write_count_;

    OCS_STATUS_RETURN_ON_ERROR(
        storage_.write("write_count", &write_count_, sizeof(write_count_)));

    OCS_STATUS_RETURN_ON_ERROR(
        storage_.write("prev_state", &prev_state_, sizeof(prev_state_)));

    OCS_STATUS_RETURN_ON_ERROR(
        storage_.write("prev_state_dur", &prev_state_dur_, sizeof(prev_state_dur_)));

    OCS_STATUS_RETURN_ON_ERROR(
        storage_.write("curr_state", &curr_state_, sizeof(curr_state_)));

    OCS_STATUS_RETURN_ON_ERROR(
        storage_.write("curr_state_dur", &curr_state_dur_, sizeof(curr_state_dur_)));

    return status::StatusCode::OK;
}

status::StatusCode FsmBlock::read_() {
    OCS_STATUS_RETURN_ON_ERROR(
        storage_.read("write_count", &write_count_, sizeof(write_count_)));

    OCS_STATUS_RETURN_ON_ERROR(
        storage_.read("prev_state", &prev_state_, sizeof(prev_state_)));

    OCS_STATUS_RETURN_ON_ERROR(
        storage_.read("prev_state_dur", &prev_state_dur_, sizeof(prev_state_dur_)));

    OCS_STATUS_RETURN_ON_ERROR(
        storage_.read("curr_state", &curr_state_, sizeof(curr_state_)));

    OCS_STATUS_RETURN_ON_ERROR(
        storage_.read("curr_state_dur", &curr_state_dur_, sizeof(curr_state_dur_)));

    return status::StatusCode::OK;
}

} // namespace control
} // namespace ocs
