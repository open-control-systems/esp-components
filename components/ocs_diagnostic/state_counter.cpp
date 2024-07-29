/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "esp_log.h"

#include "freertos/FreeRTOSConfig.h"

#include "ocs_diagnostic/state_counter.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace diagnostic {

namespace {

const char* log_tag = "state-counter";

} // namespace

StateCounter::StateCounter(storage::IStorage& storage,
                           core::IClock& clock,
                           const char* id,
                           core::microseconds_t resolution,
                           State required_state)
    : required_state_(required_state)
    , resolution_(resolution) {
    configASSERT(required_state_);

    time_counter_.reset(new (std::nothrow) TimeCounter(clock, id, resolution_));

    persistent_counter_.reset(new (std::nothrow)
                                  PersistentCounter(storage, *time_counter_));

    counter_ = persistent_counter_.get();
}

const char* StateCounter::id() const {
    return counter_->id();
}

ICounter::Value StateCounter::get() const {
    if (alive_) {
        return counter_->get();
    }

    return last_value_;
}

void StateCounter::handle_reboot() {
    persistent_counter_->handle_reboot();
}

void StateCounter::update(StateCounter::State state) {
    if (current_state_ == state) {
        return;
    }

    if (state == required_state_) {
        handle_state_set_();
    } else {
        handle_state_lost_();
    }

    ESP_LOGI(log_tag, "state changed: cur=%u new=%u req=%u alive=%d", current_state_,
             state, required_state_, alive_);

    current_state_ = state;
}

void StateCounter::handle_state_set_() {
    const auto code = persistent_counter_->invalidate();
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        ESP_LOGE(log_tag, "failed to invalidate counter value: id=%s code=%s",
                 counter_->id(), status::code_to_str(code));
    }

    time_counter_->reset(last_value_ * resolution_);

    last_value_ = 0;
    alive_ = true;
}

void StateCounter::handle_state_lost_() {
    const auto code = persistent_counter_->save();
    if (code != status::StatusCode::OK) {
        ESP_LOGE(log_tag, "failed to persist counter value: id=%s code=%s",
                 counter_->id(), status::code_to_str(code));
    }

    last_value_ = counter_->get();
    alive_ = false;
}

} // namespace diagnostic
} // namespace ocs
