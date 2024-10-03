/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <string>
#include <unordered_map>
#include <vector>

#include "ocs_control/fsm_block.h"
#include "ocs_control/ifsm_handler.h"
#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"

#pragma once

namespace ocs {
namespace control {

class FsmStore : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p block holding the FSM state.
    //!  - @p id to distinguish one store from another.
    FsmStore(FsmBlock& block, const char* id);

    //! Run FSM handlers.
    status::StatusCode run() override;

    //! Add @p handler to handle @p state of FSM.
    status::StatusCode add(FsmBlock::State state, IFsmHandler& handler);

private:
    using HandlerList = std::vector<IFsmHandler*>;

    void update_state_duration_();
    void handle_transit_begin_();
    void handle_state_();
    void handle_transit_end_();

    const std::string log_tag_;

    FsmBlock& block_;

    bool is_transit_begin_ { false };

    std::unordered_map<FsmBlock::State, HandlerList> state_to_handlers_;
};

} // namespace control
} // namespace ocs
